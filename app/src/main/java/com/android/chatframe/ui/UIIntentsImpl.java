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
package com.android.chatframe.ui;

import com.android.chatframe.MainActivity;
import com.android.chatframe.R;
import com.android.chatframe.util.LogUtil;
import com.android.chatframe.util.UiUtils;
import com.android.chatframe.util.UriUtil;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.Intents;
import android.support.v4.app.TaskStackBuilder;

/**
 * A central repository of Intents used to start activities.
 */
public class UIIntentsImpl extends UIIntents {

    @Override
    public void launchGamePlazaActivity(final Context context,Bundle bundle) {
        final Intent intent = new Intent(context,GamePlazaActivity.class);
        intent.putExtra(UIIntents.LOGIN_FINISH_INFO,bundle);
        startExternalActivity(context,intent);
    }

    @Override
    public void launchGameRoomActivity(final Context context,Bundle bundle) {
        final Intent intent = new Intent(context,GameRoomActivity.class);
        intent.putExtra(UIIntents.LOGIN_FINISH_INFO,bundle);
        startExternalActivity(context,intent);
    }

    /**
     * Provides a safe way to handle external activities which may not exist.
     */
    private void startExternalActivity(final Context context, final Intent intent) {
        try {
            context.startActivity(intent);
        } catch (final ActivityNotFoundException ex) {
            LogUtil.w(LogUtil.BUGLE_TAG, "Couldn't find activity:", ex);
            UiUtils.showToastAtBottom(R.string.activity_not_found_message);
        }
    }


}
