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


public interface RILConstants {
    // From the top of ril.cpp
    int RIL_ERRNO_INVALID_RESPONSE = -1;

    int MAX_INT = 0x7FFFFFFF;

    // from RIL_Errno
    int SUCCESS = 0;
    int RADIO_NOT_AVAILABLE = 1;
    int GENERIC_FAILURE = 2;


    int RIL_REQUEST_DIAL = 100;
    int RIL_REQUEST_HANGUP = 101;
    int RIL_REQUEST_UDUB = 102;
    int RIL_REQUEST_ANSWER = 103;
    int RIL_REQUEST_SEND_SMS = 104;


    int RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED = 1000;
    int RIL_UNSOL_RESPONSE_NEW_SMS = 1001;
    int RIL_UNSOL_RESPONSE_CONNECT_STATE_CHANGED = 1002;
    int RIL_UNSOL_CALL_RING = 1003;

}
