/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.chatframe.framework;

import static com.android.chatframe.framework.RILConstants.*;

import android.app.ActivityManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.display.DisplayManager;
import android.net.ConnectivityManager;
import android.net.LocalSocket;
import android.net.LocalSocketAddress;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.Parcel;
import android.os.PowerManager;
import com.android.chatframe.framework.Registrant;
import com.android.chatframe.framework.RegistrantList;
import android.os.RemoteException;
import android.os.PowerManager.WakeLock;
import com.android.chatframe.framework.Registrant;
import android.os.SystemClock;
import android.provider.Settings.SettingNotFoundException;
import android.telephony.CellInfo;
import android.telephony.NeighboringCellInfo;
import android.telephony.SignalStrength;
import android.telephony.SmsManager;
import android.telephony.SmsMessage;
import android.telephony.SubscriptionManager;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.SparseArray;
import android.view.Display;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.FileDescriptor;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.Random;

class RILRequest {
    static final String LOG_TAG = "RilRequest";

    //***** Class Variables
    static Random sRandom = new Random();
    static AtomicInteger sNextSerial = new AtomicInteger(0);
    private static Object sPoolSync = new Object();
    private static RILRequest sPool = null;
    private static int sPoolSize = 0;
    private static final int MAX_POOL_SIZE = 4;
    private Context mContext;

    //***** Instance Variables
    int mSerial;
    int mRequest;
    Message mResult;
    Parcel mParcel;
    RILRequest mNext;

    /**
     * Retrieves a new RILRequest instance from the pool.
     *
     * @param request RIL_REQUEST_*
     * @param result sent when operation completes
     * @return a RILRequest instance from the pool.
     */
    static RILRequest obtain(int request, Message result) {
        RILRequest rr = null;

        synchronized(sPoolSync) {
            if (sPool != null) {
                rr = sPool;
                sPool = rr.mNext;
                rr.mNext = null;
                sPoolSize--;
            }
        }

        if (rr == null) {
            rr = new RILRequest();
        }

        rr.mSerial = sNextSerial.getAndIncrement();

        rr.mRequest = request;
        rr.mResult = result;
        rr.mParcel = Parcel.obtain();

        if (result != null && result.getTarget() == null) {
            throw new NullPointerException("Message target must not be null");
        }

        // first elements in any RIL Parcel
        rr.mParcel.writeInt(request);
        rr.mParcel.writeInt(rr.mSerial);

        return rr;
    }

    /**
     * Returns a RILRequest instance to the pool.
     *
     * Note: This should only be called once per use.
     */
    void release() {
        synchronized (sPoolSync) {
            if (sPoolSize < MAX_POOL_SIZE) {
                mNext = sPool;
                sPool = this;
                sPoolSize++;
                mResult = null;
            }
        }
    }

    private RILRequest() {
    }

    static void resetSerial() {
        // use a random so that on recovery we probably don't mix old requests
        // with new.
        sNextSerial.set(sRandom.nextInt());
    }

    String serialString() {
        //Cheesy way to do %04d
        StringBuilder sb = new StringBuilder(8);
        String sn;

        long adjustedSerial = (((long)mSerial) - Integer.MIN_VALUE)%10000;

        sn = Long.toString(adjustedSerial);

        //sb.append("J[");
        sb.append('[');
        for (int i = 0, s = sn.length() ; i < 4 - s; i++) {
            sb.append('0');
        }

        sb.append(sn);
        sb.append(']');
        return sb.toString();
    }

    void onError(int error, Object ret) {
        CommandException ex;

        ex = CommandException.fromRilErrno(error);
        if (RIL.RILJ_LOGD) android.util.Log.d(LOG_TAG, serialString() + "< "
            + RIL.requestToString(mRequest)
            + " error: " + error + " ret=" + RIL.retToString(mRequest, ret));

        if (mResult != null) {
            mResult.obj = ret;
            Bundle b = new Bundle();
            b.putSerializable("CommandException",ex);
            mResult.setData(b);
            mResult.sendToTarget();
        }

        if (mParcel != null) {
            mParcel.recycle();
            mParcel = null;
        }
    }
}


/**
 * RIL implementation of the CommandsInterface.
 */
public final class RIL extends BaseCommands implements CommandsInterface {
    static final String RILJ_LOG_TAG = "Local-Rild";
    static final boolean RILJ_LOGD = true;
    static final boolean RILJ_LOGV = false; // STOPSHIP if true

    /**
     * Wake lock timeout should be longer than the longest timeout in
     * the vendor ril.
     */
    private static final int DEFAULT_WAKE_LOCK_TIMEOUT = 60000;

    //***** Instance Variables
    LocalSocket mSocket;
    HandlerThread mSenderThread;
    RILSender mSender;
    Thread mReceiverThread;
    RILReceiver mReceiver;
    WakeLock mWakeLock;
    final int mWakeLockTimeout;
    // The number of wakelock requests currently active.  Don't release the lock
    // until dec'd to 0
    int mWakeLockCount;

    SparseArray<RILRequest> mRequestList = new SparseArray<RILRequest>();

    Object     mLastNITZTimeInfo;

    private Integer mInstanceId;

    //***** Events

    static final int EVENT_SEND                 = 1;
    static final int EVENT_WAKE_LOCK_TIMEOUT    = 2;

    //***** Constants

    // match with constant in ril.cpp
    static final int RIL_MAX_COMMAND_BYTES = (8 * 1024);
    static final int RESPONSE_SOLICITED = 0;
    static final int RESPONSE_UNSOLICITED = 1;

    static final String[] SOCKET_NAME_RIL = {"localsocketrild", "localsocketrild2", "localsocketrild3"};

    static final int SOCKET_OPEN_RETRY_MILLIS = 4 * 1000;

    class RILSender extends Handler implements Runnable {
        public RILSender(Looper looper) {
            super(looper);
        }

        // Only allocated once
        byte[] dataLength = new byte[4];

        //***** Runnable implementation
        @Override
        public void run() {
            //setup if needed
        }


        //***** Handler implementation
        @Override
        public void handleMessage(Message msg) {
            RILRequest rr = (RILRequest)(msg.obj);
            RILRequest req = null;

            switch (msg.what) {
                case EVENT_SEND:
                    try {
                        LocalSocket s;

                        s = mSocket;

                        if (s == null) {
                            rr.onError(RADIO_NOT_AVAILABLE, null);
                            rr.release();
                            decrementWakeLock();
                            return;
                        }

                        synchronized (mRequestList) {
                            mRequestList.append(rr.mSerial, rr);
                        }

                        byte[] data;

                        data = rr.mParcel.marshall();
                        rr.mParcel.recycle();
                        rr.mParcel = null;

                        if (data.length > RIL_MAX_COMMAND_BYTES) {
                            throw new RuntimeException(
                                    "Parcel larger than max bytes allowed! "
                                                          + data.length);
                        }

                        // parcel length in big endian
                        dataLength[0] = dataLength[1] = 0;
                        dataLength[2] = (byte)((data.length >> 8) & 0xff);
                        dataLength[3] = (byte)((data.length) & 0xff);

                        android.util.Log.d(RILJ_LOG_TAG, "writing packet: " + data.length + " bytes");
                        for(int i=0;i<4;i++){
                            android.util.Log.d(RILJ_LOG_TAG, "writing packet: dataLength[" + i+"]: " + dataLength[i]);
                        }
                        s.getOutputStream().write(dataLength);
                        s.getOutputStream().write(data);
                    } catch (IOException ex) {
                        android.util.Log.e(RILJ_LOG_TAG, "IOException", ex);
                        req = findAndRemoveRequestFromList(rr.mSerial);
                        // make sure this request has not already been handled,
                        // eg, if RILReceiver cleared the list.
                        if (req != null) {
                            rr.onError(RADIO_NOT_AVAILABLE, null);
                            rr.release();
                            decrementWakeLock();
                        }
                    } catch (RuntimeException exc) {
                        android.util.Log.e(RILJ_LOG_TAG, "Uncaught exception ", exc);
                        req = findAndRemoveRequestFromList(rr.mSerial);
                        // make sure this request has not already been handled,
                        // eg, if RILReceiver cleared the list.
                        if (req != null) {
                            rr.onError(GENERIC_FAILURE, null);
                            rr.release();
                            decrementWakeLock();
                        }
                    }

                    break;

                case EVENT_WAKE_LOCK_TIMEOUT:
                    // Haven't heard back from the last request.  Assume we're
                    // not getting a response and  release the wake lock.

                    // The timer of WAKE_LOCK_TIMEOUT is reset with each
                    // new send request. So when WAKE_LOCK_TIMEOUT occurs
                    // all requests in mRequestList already waited at
                    // least DEFAULT_WAKE_LOCK_TIMEOUT but no response.
                    //
                    // Note: Keep mRequestList so that delayed response
                    // can still be handled when response finally comes.

                    synchronized (mRequestList) {
                        if (clearWakeLock()) {
                            if (RILJ_LOGD) {
                                int count = mRequestList.size();
                                android.util.Log.d(RILJ_LOG_TAG, "WAKE_LOCK_TIMEOUT " +
                                        " mRequestList=" + count);
                                for (int i = 0; i < count; i++) {
                                    rr = mRequestList.valueAt(i);
                                    android.util.Log.d(RILJ_LOG_TAG, i + ": [" + rr.mSerial + "] "
                                            + requestToString(rr.mRequest));
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }

    /**
     * Reads in a single RIL message off the wire. A RIL message consists
     * of a 4-byte little-endian length and a subsequent series of bytes.
     * The final message (length header omitted) is read into
     * <code>buffer</code> and the length of the final message (less header)
     * is returned. A return value of -1 indicates end-of-stream.
     *
     * @param is non-null; Stream to read from
     * @param buffer Buffer to fill in. Must be as large as maximum
     * message size, or an ArrayOutOfBounds exception will be thrown.
     * @return Length of message less header, or -1 on end of stream.
     * @throws IOException
     */
    private static int readRilMessage(InputStream is, byte[] buffer)
            throws IOException {
        int countRead;
        int offset;
        int remaining;
        int messageLength;

        // First, read in the length of the message
        offset = 0;
        remaining = 4;
        do {
            countRead = is.read(buffer, offset, remaining);
            android.util.Log.d(RILJ_LOG_TAG, "reading message.  countRead=" + countRead);
            if (countRead < 0 ) {
                android.util.Log.e(RILJ_LOG_TAG, "Hit EOS reading message length");
                return -1;
            }

            offset += countRead;
            remaining -= countRead;
        } while (remaining > 0);

        android.util.Log.d(RILJ_LOG_TAG, "reading message.  buffer[0]=" + buffer[0]+"   buffer[1]="+buffer[1]+"   buffer[2]="+buffer[2]+"  buffer[3]="+buffer[3]);

//        messageLength = ((buffer[3] & 0xff) << 24)
//                | ((buffer[2] & 0xff) << 16)
//                | ((buffer[1] & 0xff) << 8)
//                | (buffer[0] & 0xff);
        messageLength = ((buffer[0] & 0xff) << 24)
                | ((buffer[1] & 0xff) << 16)
                | ((buffer[2] & 0xff) << 8)
                | (buffer[3] & 0xff);
        android.util.Log.d(RILJ_LOG_TAG, "reading message.  messageLength=" + messageLength);
        // Then, re-use the buffer and read in the message itself
        offset = 0;
        remaining = messageLength;
        do {
            countRead = is.read(buffer, offset, remaining);

            if (countRead < 0 ) {
                android.util.Log.e(RILJ_LOG_TAG, "Hit EOS reading message.  messageLength=" + messageLength
                        + " remaining=" + remaining);
                return -1;
            }

            offset += countRead;
            remaining -= countRead;
        } while (remaining > 0);

        return messageLength;
    }

    class RILReceiver implements Runnable {
        byte[] buffer;

        RILReceiver() {
            buffer = new byte[RIL_MAX_COMMAND_BYTES];
        }

        @Override
        public void run() {
            int retryCount = 0;
            String rilSocket = "localsocketrild";

            try {
                for (;;) {
                    LocalSocket s = null;
                    LocalSocketAddress l;

                    if (mInstanceId == null || mInstanceId == 0) {
                        rilSocket = SOCKET_NAME_RIL[0];
                    } else {
                        rilSocket = SOCKET_NAME_RIL[mInstanceId];
                    }

                    try {
                        s = new LocalSocket();
                        l = new LocalSocketAddress(rilSocket,
                                LocalSocketAddress.Namespace.ABSTRACT);
                        s.connect(l);
                    } catch (IOException ex) {
                        try {
                            if (s != null) {
                                s.close();
                            }
                        } catch (IOException ex2) {
                            //ignore failure to close after failure to connect
                        }

                        // don't print an error message after the the first time
                        // or after the 8th time

                        if (retryCount == 8) {
                            android.util.Log.e(RILJ_LOG_TAG,
                                    "Couldn't find '" + rilSocket
                                            + "' socket after " + retryCount
                                            + " times, continuing to retry silently");
                        } else if (retryCount >= 0 && retryCount < 8) {
                            android.util.Log.i(RILJ_LOG_TAG,
                                    "Couldn't find '" + rilSocket
                                            + "' socket; retrying after timeout");
                        }

                        try {
                            Thread.sleep(SOCKET_OPEN_RETRY_MILLIS);
                        } catch (InterruptedException er) {
                        }

                        retryCount++;
                        continue;
                    }

                    retryCount = 0;

                    mSocket = s;
                    android.util.Log.d(RILJ_LOG_TAG, "(" + mInstanceId + ") Connected to '"
                            + rilSocket + "' socket");

                    int length = 0;
                    try {
                        InputStream is = mSocket.getInputStream();

                        for (; ; ) {
                            Parcel p;

                            length = readRilMessage(is, buffer);

                            if (length < 0) {
                                // End-of-stream reached
                                break;
                            }

                            p = Parcel.obtain();
                            p.unmarshall(buffer, 0, length);
                            p.setDataPosition(0);

                            android.util.Log.d(RILJ_LOG_TAG, "Read packet: " + length + " bytes");

                            processResponse(p);
                            p.recycle();
                        }
                    } catch (java.io.IOException ex) {
                        android.util.Log.i(RILJ_LOG_TAG, "'" + rilSocket + "' socket closed",
                                ex);
                    } catch (Throwable tr) {
                        android.util.Log.e(RILJ_LOG_TAG, "Uncaught exception read length=" + length +
                                "Exception:" + tr.toString());
                    }

                    android.util.Log.i(RILJ_LOG_TAG, "(" + mInstanceId + ") Disconnected from '" + rilSocket
                            + "' socket");

                    try {
                        mSocket.close();
                    } catch (IOException ex) {
                    }

                    mSocket = null;
                    RILRequest.resetSerial();

                    // Clear request list on close
                    clearRequestList(RADIO_NOT_AVAILABLE, false);
                }
            } catch (Throwable tr) {
                android.util.Log.e(RILJ_LOG_TAG,"Uncaught exception", tr);
            }

        }
    }

    //***** Constructors
    public RIL(Context context) {
        this(context, null);
    }

    public RIL(Context context, Integer instanceId) {
        super(context);

        mContext = context;

        mInstanceId = instanceId;

        PowerManager pm = (PowerManager)context.getSystemService(Context.POWER_SERVICE);
        mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, RILJ_LOG_TAG);
        mWakeLock.setReferenceCounted(false);
        mWakeLockTimeout = DEFAULT_WAKE_LOCK_TIMEOUT;
        mWakeLockCount = 0;

        mSenderThread = new HandlerThread("RILSender" + mInstanceId);
        mSenderThread.start();

        Looper looper = mSenderThread.getLooper();
        mSender = new RILSender(looper);

    }

    @Override
    public void startRild(){
        ConnectivityManager cm = (ConnectivityManager)mContext.getSystemService(
                Context.CONNECTIVITY_SERVICE);
//        if (!cm.getActiveNetworkInfo().isConnectedOrConnecting()) {
//            riljLog("Not starting RILReceiver: wifi or 3G is not connected");
//        } else {
        riljLog("Starting RILReceiver " + mInstanceId);
        mReceiver = new RILReceiver();
        mReceiverThread = new Thread(mReceiver, "RILReceiver" + mInstanceId);
        mReceiverThread.start();
        //       }
    }

    private void constructGsmSendSmsRilRequest (RILRequest rr, String smscPDU, String pdu) {
        rr.mParcel.writeInt(2);
        rr.mParcel.writeString(smscPDU);
        rr.mParcel.writeString(pdu);
    }

    @Override
    public void sendSMS (String smscPDU, String pdu, Message result) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_SEND_SMS, result);

        constructGsmSendSmsRilRequest(rr, smscPDU, pdu);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        send(rr);
    }

    @Override
    public void dial(String address, int clirMode, Message result) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_DIAL, result);
        rr.mParcel.writeString(address);
        rr.mParcel.writeInt(clirMode);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        send(rr);
    }

    @Override
    public void hangupConnection (int gsmIndex, Message result) {
        RILRequest rr = RILRequest.obtain(RIL_REQUEST_HANGUP, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest) + " " +
                gsmIndex);

        rr.mParcel.writeInt(1);
        rr.mParcel.writeInt(gsmIndex);

        send(rr);
    }

    @Override
    public void acceptCall (Message result) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_ANSWER, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        send(rr);
    }

    @Override
    public void rejectCall(Message result) {
        RILRequest rr
                = RILRequest.obtain(RIL_REQUEST_UDUB, result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        send(rr);
    }

    private void constructCdmaSendSmsRilRequest(RILRequest rr, byte[] pdu) {
        int address_nbr_of_digits;
        int subaddr_nbr_of_digits;
        int bearerDataLength;
        ByteArrayInputStream bais = new ByteArrayInputStream(pdu);
        DataInputStream dis = new DataInputStream(bais);

        try {
            rr.mParcel.writeInt(dis.readInt()); //teleServiceId
            rr.mParcel.writeByte((byte) dis.readInt()); //servicePresent
            rr.mParcel.writeInt(dis.readInt()); //serviceCategory
            rr.mParcel.writeInt(dis.read()); //address_digit_mode
            rr.mParcel.writeInt(dis.read()); //address_nbr_mode
            rr.mParcel.writeInt(dis.read()); //address_ton
            rr.mParcel.writeInt(dis.read()); //address_nbr_plan
            address_nbr_of_digits = (byte) dis.read();
            rr.mParcel.writeByte((byte) address_nbr_of_digits);
            for(int i=0; i < address_nbr_of_digits; i++){
                rr.mParcel.writeByte(dis.readByte()); // address_orig_bytes[i]
            }
            rr.mParcel.writeInt(dis.read()); //subaddressType
            rr.mParcel.writeByte((byte) dis.read()); //subaddr_odd
            subaddr_nbr_of_digits = (byte) dis.read();
            rr.mParcel.writeByte((byte) subaddr_nbr_of_digits);
            for(int i=0; i < subaddr_nbr_of_digits; i++){
                rr.mParcel.writeByte(dis.readByte()); //subaddr_orig_bytes[i]
            }

            bearerDataLength = dis.read();
            rr.mParcel.writeInt(bearerDataLength);
            for(int i=0; i < bearerDataLength; i++){
                rr.mParcel.writeByte(dis.readByte()); //bearerData[i]
            }
        }catch (IOException ex){
            if (RILJ_LOGD) riljLog("sendSmsCdma: conversion from input stream to object failed: "
                    + ex);
        }
    }

    /**
     * Holds a PARTIAL_WAKE_LOCK whenever
     * a) There is outstanding RIL request sent to RIL deamon and no replied
     * b) There is a request pending to be sent out.
     *
     * There is a WAKE_LOCK_TIMEOUT to release the lock, though it shouldn't
     * happen often.
     */

    private void acquireWakeLock() {
        synchronized (mWakeLock) {
            mWakeLock.acquire();
            mWakeLockCount++;

            mSender.removeMessages(EVENT_WAKE_LOCK_TIMEOUT);
            Message msg = mSender.obtainMessage(EVENT_WAKE_LOCK_TIMEOUT);
            mSender.sendMessageDelayed(msg, mWakeLockTimeout);
        }
    }

    private void decrementWakeLock() {
        synchronized (mWakeLock) {
            if (mWakeLockCount > 1) {
                mWakeLockCount--;
            } else {
                mWakeLockCount = 0;
                mWakeLock.release();
                mSender.removeMessages(EVENT_WAKE_LOCK_TIMEOUT);
            }
        }
    }

    // true if we had the wakelock
    private boolean clearWakeLock() {
        synchronized (mWakeLock) {
            if (mWakeLockCount == 0 && mWakeLock.isHeld() == false) return false;
            android.util.Log.d(RILJ_LOG_TAG, "NOTE: mWakeLockCount is " + mWakeLockCount + "at time of clearing");
            mWakeLockCount = 0;
            mWakeLock.release();
            mSender.removeMessages(EVENT_WAKE_LOCK_TIMEOUT);
            return true;
        }
    }

    private void send(RILRequest rr) {
        Message msg;

        if (mSocket == null) {
            rr.onError(RADIO_NOT_AVAILABLE, null);
            rr.release();
            return;
        }

        msg = mSender.obtainMessage(EVENT_SEND, rr);

        acquireWakeLock();

        msg.sendToTarget();
    }

    private void processResponse (Parcel p) {
        int type;

        type = p.readInt();
        android.util.Log.d(RILJ_LOG_TAG, "=====processResponse===Read type: " + type);
        if (type == RESPONSE_UNSOLICITED) {
            processUnsolicited (p);
        } else if (type == RESPONSE_SOLICITED) {
            RILRequest rr = processSolicited (p);
            if (rr != null) {
                rr.release();
                decrementWakeLock();
            }
        }
    }

    /**
     * Release each request in mRequestList then clear the list
     * @param error is the RIL_Errno sent back
     * @param loggable true means to print all requests in mRequestList
     */
    private void clearRequestList(int error, boolean loggable) {
        RILRequest rr;
        synchronized (mRequestList) {
            int count = mRequestList.size();
            if (RILJ_LOGD && loggable) {
                android.util.Log.d(RILJ_LOG_TAG, "clearRequestList " +
                        " mWakeLockCount=" + mWakeLockCount +
                        " mRequestList=" + count);
            }

            for (int i = 0; i < count ; i++) {
                rr = mRequestList.valueAt(i);
                if (RILJ_LOGD && loggable) {
                    android.util.Log.d(RILJ_LOG_TAG, i + ": [" + rr.mSerial + "] " +
                            requestToString(rr.mRequest));
                }
                rr.onError(error, null);
                rr.release();
                decrementWakeLock();
            }
            mRequestList.clear();
        }
    }

    private RILRequest findAndRemoveRequestFromList(int serial) {
        RILRequest rr = null;
        synchronized (mRequestList) {
            rr = mRequestList.get(serial);
            if (rr != null) {
                mRequestList.remove(serial);
            }
        }

        return rr;
    }

    private RILRequest processSolicited (Parcel p) {
        int serial, error;

        serial = p.readInt();
        error = p.readInt();
        android.util.Log.d(RILJ_LOG_TAG, "=====processSolicited===Read serial: " + serial+"     error: "+error);
        RILRequest rr;

        rr = findAndRemoveRequestFromList(serial);

        if (rr == null) {
            android.util.Log.w(RILJ_LOG_TAG, "Unexpected solicited response! sn: "
                            + serial + " error: " + error);
            return null;
        }

        Object ret = null;

        if (error == 0 || p.dataAvail() > 0) {
            // either command succeeds or command fails but with data payload
            try {
                switch (rr.mRequest) {
                    case RIL_REQUEST_DIAL:
                        ret = responseDial(p);
                        break;
                    case RIL_REQUEST_HANGUP:
                        ret = responseHangup(p);
                        break;
                    case RIL_REQUEST_UDUB:
                        ret = responseReject(p);
                        break;
                    case RIL_REQUEST_ANSWER:
                        ret = responseAnswer(p);
                        break;
                    case RIL_REQUEST_SEND_SMS:
                        ret = responseSMS(p);
                        break;
                    default:
                        throw new RuntimeException("Unrecognized solicited response: " + rr.mRequest);
                }
            } catch (Throwable tr) {
                // Exceptions here usually mean invalid RIL responses

                android.util.Log.w(RILJ_LOG_TAG, rr.serialString() + "< "
                        + requestToString(rr.mRequest)
                        + " exception, possible invalid RIL response", tr);

                if (rr.mResult != null) {
                    Bundle b = new Bundle();
                    b.putSerializable("Throwable",tr);
                    rr.mResult.setData(b);
                    rr.mResult.sendToTarget();
                }
                return rr;
            }
        }

        if (error != 0) {
            if (error != 0) rr.onError(error, ret);
        }
        if (error == 0) {
            if (RILJ_LOGD) riljLog(rr.serialString() + "< " + requestToString(rr.mRequest)
                    + " " + retToString(rr.mRequest, ret));

            if (rr.mResult != null) {
                rr.mResult.obj = ret;
                rr.mResult.sendToTarget();
            }
        }
        return rr;
    }

    static String retToString(int req, Object ret) {
        if (ret == null) return "";
        StringBuilder sb;
        String s;
        int length;
        if (ret instanceof int[]){
            int[] intArray = (int[]) ret;
            length = intArray.length;
            sb = new StringBuilder("{");
            if (length > 0) {
                int i = 0;
                sb.append(intArray[i++]);
                while ( i < length) {
                    sb.append(", ").append(intArray[i++]);
                }
            }
            sb.append("}");
            s = sb.toString();
        } else if (ret instanceof String[]) {
            String[] strings = (String[]) ret;
            length = strings.length;
            sb = new StringBuilder("{");
            if (length > 0) {
                int i = 0;
                sb.append(strings[i++]);
                while ( i < length) {
                    sb.append(", ").append(strings[i++]);
                }
            }
            sb.append("}");
            s = sb.toString();
        }else {
            s = ret.toString();
        }
        return s;
    }

    private void processUnsolicited(Parcel p) {
        int response;
        Object ret;

        response = p.readInt();
        android.util.Log.d(RILJ_LOG_TAG, "=====processUnsolicited===Read response: " + response);
        try {
            switch (response) {
                case RIL_UNSOL_RESPONSE_NEW_SMS:
                    ret = responseString(p);
                    break;
                case RIL_UNSOL_RESPONSE_CONNECT_STATE_CHANGED:
                    ret = responseString(p);
                    break;
                case RIL_UNSOL_CALL_RING:
                    ret = responseCallRing(p);
                    break;
                case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED:
                    ret =  responseVoid(p);
                    break;
                default:
                    throw new RuntimeException("Unrecognized unsol response: " + response+"    error: "+responseString(p));
            }

        } catch (Throwable tr) {
            android.util.Log.e(RILJ_LOG_TAG, "Exception processing unsol response: " + response +
                    "\nException:" + tr.toString());
            return;
        }

        switch (response) {
            case RIL_UNSOL_RESPONSE_NEW_SMS: {
                if (RILJ_LOGD) unsljLog(response);

                if (mGsmSmsRegistrant != null) {
                    mGsmSmsRegistrant
                            .notifyRegistrant(ret,null);
                }
                break;
            }
            case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED:{
                if (RILJ_LOGD) unsljLog(response);

                mCallStateRegistrants
                        .notifyRegistrants();
                break;
            }
            case RIL_UNSOL_CALL_RING:{
                if (RILJ_LOGD) unsljLogRet(response, ret);

                if (mRingRegistrant != null) {
                    mRingRegistrant.notifyResult(ret);
                }
                break;
            }
        }
    }

    private Object responseInts(Parcel p) {
        int numInts;
        int response[];

        numInts = p.readInt();

        response = new int[numInts];

        for (int i = 0 ; i < numInts ; i++) {
            response[i] = p.readInt();
        }

        return response;
    }

    private Object responseVoid(Parcel p) {
        return null;
    }

    private Object responseString(Parcel p) {
        String response;
        int pos = p.dataPosition();
        int size = p.dataSize();
        android.util.Log.d(RILJ_LOG_TAG, "=====responseString===pos : "+pos+"     size: " + size);
        response = p.readString();
        android.util.Log.d(RILJ_LOG_TAG, "=====responseString==Read response: " + response);
        return response;
    }

    private Object responseStrings(Parcel p) {
        int num = p.readInt();
        String response[] = new String[num];
        p.readStringArray(response);

        return response;
    }

    private Object responseSMS(Parcel p) {
        String ret;
        ret = p.readString();
        android.util.Log.d(RILJ_LOG_TAG, "=====responseSMS==Read response: " + ret);
        return ret;
    }

    private Object responseDial(Parcel p){
        return responseString(p);
    }

    private Object responseHangup(Parcel p){
        return responseString(p);
    }

    private Object responseAnswer(Parcel p){
        return responseString(p);
    }

    private Object responseReject(Parcel p){
        return responseString(p);
    }

    private Object responseCallRing(Parcel p){
        return p.readString();
    }

    static String requestToString(int request) {
        switch(request) {
            case RIL_REQUEST_DIAL: return "DIAL";
            case RIL_REQUEST_HANGUP: return "HANGUP";
            case RIL_REQUEST_UDUB: return "REJECT";
            case RIL_REQUEST_ANSWER: return "ANSWER";
            case RIL_REQUEST_SEND_SMS: return "SEND_SMS";
            default: return "<unknown request>";
        }
    }

    static String responseToString(int request) {
        switch(request) {
            case RIL_UNSOL_RESPONSE_NEW_SMS: return "UNSOL_RESPONSE_NEW_SMS";
            case RIL_UNSOL_CALL_RING: return "UNSOL_CALL_RING";
            case RIL_UNSOL_RESPONSE_CONNECT_STATE_CHANGED: return "UNSOL_RESPONSE_CONNECT";
            case RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED: return "UNSOL_RESPONSE_CALL_STATE_CHANGED";
            default: return "<unknown response>";
        }
    }

    private void riljLog(String msg) {
        android.util.Log.d(RILJ_LOG_TAG, msg
                + (mInstanceId != null ? (" [SUB" + mInstanceId + "]") : ""));
    }

    private void riljLogv(String msg) {
        android.util.Log.v(RILJ_LOG_TAG, msg
                + (mInstanceId != null ? (" [SUB" + mInstanceId + "]") : ""));
    }

    private void unsljLog(int response) {
        riljLog("[UNSL]< " + responseToString(response));
    }

    private void unsljLogMore(int response, String more) {
        riljLog("[UNSL]< " + responseToString(response) + " " + more);
    }

    private void unsljLogRet(int response, Object ret) {
        riljLog("[UNSL]< " + responseToString(response) + " " + retToString(response, ret));
    }

    private void unsljLogvRet(int response, Object ret) {
        riljLogv("[UNSL]< " + responseToString(response) + " " + retToString(response, ret));
    }

    public void dump(FileDescriptor fd, PrintWriter pw, String[] args) {
        pw.println("RIL: " + this);
        pw.println(" mSocket=" + mSocket);
        pw.println(" mSenderThread=" + mSenderThread);
        pw.println(" mSender=" + mSender);
        pw.println(" mReceiverThread=" + mReceiverThread);
        pw.println(" mReceiver=" + mReceiver);
        pw.println(" mWakeLock=" + mWakeLock);
        pw.println(" mWakeLockTimeout=" + mWakeLockTimeout);
        synchronized (mRequestList) {
            synchronized (mWakeLock) {
                pw.println(" mWakeLockCount=" + mWakeLockCount);
            }
            int count = mRequestList.size();
            pw.println(" mRequestList count=" + count);
            for (int i = 0; i < count; i++) {
                RILRequest rr = mRequestList.valueAt(i);
                pw.println("  [" + rr.mSerial + "] " + requestToString(rr.mRequest));
            }
        }
        pw.println(" mLastNITZTimeInfo=" + mLastNITZTimeInfo);
    }

}