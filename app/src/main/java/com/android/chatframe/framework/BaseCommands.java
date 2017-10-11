
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

import android.content.Context;
import android.os.Message;
import com.android.chatframe.framework.RegistrantList;
import com.android.chatframe.framework.Registrant;
import android.os.Handler;
import android.os.Parcel;



public abstract class BaseCommands implements CommandsInterface {
    //***** Instance Variables
    protected Context mContext;
    protected Object mStateMonitor = new Object();

    protected RegistrantList mCallStateRegistrants = new RegistrantList();

    protected Registrant mGsmSmsRegistrant;
    protected Registrant mRingRegistrant;


    public BaseCommands(Context context) {
        mContext = context;  // May be null (if so we won't log statistics)
    }

    //***** CommandsInterface implementation

    @Override
    public void setOnNewGsmSms(Handler h, int what, Object obj) {
        mGsmSmsRegistrant = new Registrant (h, what, obj);
    }

    @Override
    public void unSetOnNewGsmSms(Handler h) {
        if (mGsmSmsRegistrant != null && mGsmSmsRegistrant.getHandler() == h) {
            mGsmSmsRegistrant.clear();
            mGsmSmsRegistrant = null;
        }
    }

    @Override
    public void registerForCallStateChanged(Handler h, int what, Object obj) {
        Registrant r = new Registrant (h, what, obj);
        synchronized (mStateMonitor) {
            mCallStateRegistrants.add(r);
        }
    }

    @Override
    public void unregisterForCallStateChanged(Handler h) {
        synchronized (mStateMonitor) {
            mCallStateRegistrants.remove(h);
        }
    }

    @Override
    public void setOnCallRing(Handler h, int what, Object obj) {
        mRingRegistrant = new Registrant (h, what, obj);
    }

    @Override
    public void unSetOnCallRing(Handler h) {
        if (mRingRegistrant != null && mRingRegistrant.getHandler() == h) {
            mRingRegistrant.clear();
            mRingRegistrant = null;
        }
    }

}
