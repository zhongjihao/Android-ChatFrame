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

import android.os.Bundle;
import android.os.Message;
import android.os.Handler;

public interface CommandsInterface {
    void startRild();

    void registerForRadioStateChanged(Handler h, int what, Object obj);
    void unregisterForRadioStateChanged(Handler h);


    void setOnNewGsmSms(Handler h, int what, Object obj);
    void unSetOnNewGsmSms(Handler h);

    void registerForCallStateChanged(Handler h, int what, Object obj);
    void unregisterForCallStateChanged(Handler h);

    void setOnCallRing(Handler h, int what, Object obj);
    void unSetOnCallRing(Handler h);


    void sendSMS(String smscPDU, String pdu, Message response);
    void dial(String address, int clirMode, Message result);
    void hangupConnection(int gsmIndex, Message result);
    void acceptCall(Message result);
    void rejectCall(Message result);
}
