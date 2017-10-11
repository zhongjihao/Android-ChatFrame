package com.android.chatframe.framework;

import android.content.Context;
import android.os.Handler;
import android.os.Message;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-5-3.
 */

public class MsgDispatcher extends Handler {
    private static final  String LOG_TAG = "MsgDispatcher";

    public static final int EVENT_NEW_SMS = 1;
    public static final int EVENT_SEND_SMS_COMPLETE   = 2;
    public static final int EVENT_CALL_STATE_CHANGE   = 3;
    public static final int EVENT_DIAL_COMPLETE  = 4;
    public static final int EVENT_HANGUP_COMPLETE  = 5;
    public static final int EVENT_ACCEPT_CALL_COMPLETE  = 6;
    public static final int EVENT_REJECT_CALL_COMPLETE  = 7;
    public static final int EVENT_CALL_RING  = 8;

    private final CommandsInterface mCi;
    private RequestEventListener listenr;

    public MsgDispatcher(CommandsInterface ci) {
        mCi = ci;
        mCi.registerForCallStateChanged(this, EVENT_CALL_STATE_CHANGE, null);
        mCi.setOnNewGsmSms(this, EVENT_NEW_SMS, null);
        mCi.setOnCallRing(this, EVENT_CALL_RING, null);
        mCi.startRild();
    }

    public void registerEventListener(RequestEventListener l){
        listenr = l;
    }

    public void unregisterEventListener(){
        listenr = null;
    }

    @Override
    public void handleMessage(Message msg) {
        android.util.Log.d(LOG_TAG, "MsgDispatcher msg type: "+msg.what);
        if(listenr == null) return;
        switch (msg.what) {
            case EVENT_SEND_SMS_COMPLETE:
                listenr.onSentSmsComplete(msg);
                break;
            case EVENT_NEW_SMS:
                listenr.onNewMessageArrive(msg);
                break;
            case EVENT_DIAL_COMPLETE:
                listenr.onDialComplete(msg);
                break;
            case EVENT_ACCEPT_CALL_COMPLETE:
                listenr.onAcceptComplete(msg);
                break;
            case EVENT_HANGUP_COMPLETE:
                listenr.onHangupComplete(msg);
                break;
            case EVENT_REJECT_CALL_COMPLETE:
                listenr.onRejectComplete(msg);
                break;
            case EVENT_CALL_RING:
                listenr.onIncoming(msg);
                break;

            default:
                android.util.Log.e(LOG_TAG, "handleMessage() ignoring message of unexpected type " + msg.what);
        }
    }

    public void sendSMS(String smscPDU, String pdu) {
        Message reply = obtainMessage(EVENT_SEND_SMS_COMPLETE);
        mCi.sendSMS(smscPDU,pdu,reply);
    }

    public void dial(String phone, int clirMode) {
        Message reply = obtainMessage(EVENT_DIAL_COMPLETE);
        mCi.dial(phone,clirMode,reply);
    }

    public void hangupConnection(int gsmIndex) {
        Message reply = obtainMessage(EVENT_HANGUP_COMPLETE);
        mCi.hangupConnection(gsmIndex,reply);
    }

    public void acceptCall() {
        Message reply = obtainMessage(EVENT_ACCEPT_CALL_COMPLETE);
        mCi.acceptCall(reply);
    }

    public void rejectCall() {
        Message reply = obtainMessage(EVENT_REJECT_CALL_COMPLETE);
        mCi.rejectCall(reply);
    }

    public void dispose() {
        android.util.Log.d(LOG_TAG, "MsgDispatcher dispose");
        unregisterEventListener();
        mCi.unSetOnNewGsmSms(this);
        mCi.unregisterForCallStateChanged(this);
        mCi.unSetOnCallRing(this);
    }

}
