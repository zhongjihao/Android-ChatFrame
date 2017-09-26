/*
 * Copyright (C) 2007 The Android Open Source Project
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

import com.android.chatframe.framework.RILConstants;



/**
 * {@hide}
 */
public class CommandException extends RuntimeException {
    private Error mError;

    public enum Error {
        INVALID_RESPONSE,
        RADIO_NOT_AVAILABLE,
        GENERIC_FAILURE,
    }

    public CommandException(Error e) {
        super(e.toString());
        mError = e;
    }

    public static CommandException fromRilErrno(int ril_errno) {
        switch(ril_errno) {
            case RILConstants.SUCCESS:                       return null;
            case RILConstants.RIL_ERRNO_INVALID_RESPONSE:
                return new CommandException(Error.INVALID_RESPONSE);
            case RILConstants.RADIO_NOT_AVAILABLE:
                return new CommandException(Error.RADIO_NOT_AVAILABLE);
            case RILConstants.GENERIC_FAILURE:
                return new CommandException(Error.GENERIC_FAILURE);

            default:
                android.util.Log.e("RIL", "Unrecognized RIL errno " + ril_errno);
                return new CommandException(Error.INVALID_RESPONSE);
        }
    }

    public Error getCommandError() {
        return mError;
    }

}
