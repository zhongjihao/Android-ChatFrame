/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.chatframe.datamodel.action;

import com.android.chatframe.Factory;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DatabaseWrapper;
import com.android.chatframe.util.LogUtil;
import android.content.ContentValues;
import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;


/**
 * Action used to "receive" an incoming message
 */
public class ReceiveMessageAction extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;

    private static final String KEY_MESSAGE_VALUES = "message_values";

    /**
     * Create a message received from a particular number in a particular conversation
     */
    public ReceiveMessageAction(final ContentValues messageValues) {
        actionParameters.putParcelable(KEY_MESSAGE_VALUES, messageValues);
    }

    @Override
    protected Object executeAction() {
        final Context context = Factory.get().getApplicationContext();
        final ContentValues messageValues = actionParameters.getParcelable(KEY_MESSAGE_VALUES);
        final DatabaseWrapper db = DataModel.get().getDatabase();

        // Get the SIM subscription ID
        Integer subId = messageValues.getAsInteger("sub_id");

        //TODO ...
      
     
        return null;
    }

    private ReceiveMessageAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<ReceiveMessageAction> CREATOR
            = new Parcelable.Creator<ReceiveMessageAction>() {
        @Override
        public ReceiveMessageAction createFromParcel(final Parcel in) {
            return new ReceiveMessageAction(in);
        }

        @Override
        public ReceiveMessageAction[] newArray(final int size) {
            return new ReceiveMessageAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
