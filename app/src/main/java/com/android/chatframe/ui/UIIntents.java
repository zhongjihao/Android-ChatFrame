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

import com.android.chatframe.Factory;
import android.content.Context;
import android.os.Bundle;

/**
 * A central repository of Intents used to start activities.
 */
public abstract class UIIntents {
    public static UIIntents get() {
        return Factory.get().getUIIntents();
    }

    // Intent extras
    public static final String LOGIN_FINISH_INFO = "LoginFinishInfo";

    public static final String UI_INTENT_EXTRA_LOGIN_PLAZA_SUCCESS_DATA = "login_plaza_success_data";
    public static final String UI_INTENT_EXTRA_GAMEROOM_LIST_DATA = "game_room_list_data";
    public static final String UI_INTENT_EXTRA_PLAZA_NAME_DATA = "plaza_name_data";
    public static final String UI_INTENT_EXTRA_SERVER_INFO_DATA = "server_info_data";
    public static final String UI_INTENT_EXTRA_SERVER_ONLINE_COUNT_DATA = "server_online_count_data";
    public static final String UI_INTENT_EXTRA_TAG_USER_DATA = "tag_user_data";


    public abstract void launchGamePlazaActivity(final Context context,Bundle bundle);
    public abstract void launchGameRoomActivity(final Context context,Bundle bundle);
}
