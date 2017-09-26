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

package com.android.chatframe.datamodel;

import com.android.chatframe.config.MmsConfig;
import com.android.chatframe.datamodel.action.ActionService;
import com.android.chatframe.datamodel.action.BackgroundWorker;
import com.android.chatframe.datamodel.action.FixupMessageStatusOnStartupAction;
import com.android.chatframe.datamodel.data.ConversationData;
import com.android.chatframe.datamodel.data.ConversationListData;
import com.android.chatframe.datamodel.data.ConversationListData.ConversationListDataListener;
import com.android.chatframe.datamodel.data.MediaPickerData;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.Assert.DoesNotRunOnMainThread;
import com.android.chatframe.util.ConnectivityUtil;
import com.android.chatframe.util.LogUtil;
import com.android.chatframe.util.OsUtil;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;


public class DataModelImpl extends DataModel {
    private final Context mContext;
    private final ActionService mActionService;
    private final BackgroundWorker mDataModelWorker;
    private final DatabaseHelper mDatabaseHelper;
    private final ConnectivityUtil mConnectivityUtil;

    public DataModelImpl(final Context context) {
        super();
        mContext = context;
        mActionService = new ActionService();
        mDataModelWorker = new BackgroundWorker();
        mDatabaseHelper = DatabaseHelper.getInstance(context);
        mConnectivityUtil = new ConnectivityUtil(context);
    }

    @Override
    public ConversationListData createConversationListData(final Context context,
            final ConversationListDataListener listener, final boolean archivedMode) {
        return new ConversationListData(context, listener, archivedMode);
    }

    @Override
    public ConversationData createConversationData(final Context context, final String conversationId) {
        return new ConversationData(context,conversationId);
    }

    @Override
    public MediaPickerData createMediaPickerData(final Context context) {
        return new MediaPickerData(context);
    }

    @Override
    public ActionService getActionService() {
        // We need to allow access to this on the UI thread since it's used to start actions.
        return mActionService;
    }

    @Override
    public BackgroundWorker getBackgroundWorkerForActionService() {
        return mDataModelWorker;
    }

    @Override
    @DoesNotRunOnMainThread
    public DatabaseWrapper getDatabase() {
        // We prevent the main UI thread from accessing the database since we have to allow
        // public access to this class to enable sub-packages to access data.
        Assert.isNotMainThread();
        return mDatabaseHelper.getDatabase();
    }

    @Override
    public ConnectivityUtil getConnectivityUtil() {
        return mConnectivityUtil;
    }

    @Override
    void onCreateTables(final SQLiteDatabase db) {
        LogUtil.w(LogUtil.BUGLE_TAG, "Rebuilt databases: reseting related state");
        // Clear other things that implicitly reference the DB
        //SyncManager.resetLastSyncTimestamps();
    }

    @Override
    public void onActivityResume() {
        // Perform an incremental sync and register for changes if necessary
        //mSyncManager.updateSyncObserver(mContext);

        // Trigger a participant refresh if needed, we should only need to refresh if there is
        // contact change while the activity was paused.
        //ParticipantRefresh.refreshParticipantsIfNeeded();
    }

    @Override
    public void onApplicationCreated() {
        FixupMessageStatusOnStartupAction.fixupMessageStatus();

        if (OsUtil.isAtLeastL_MR1()) {
            // Start listening for subscription change events for refreshing self participants.
          
            // TODO: This dynamically changes the mms config that app is
            // currently using. It may cause inconsistency in some cases. We need
            // to check the usage of mms config and handle the dynamic change
            // gracefully
            MmsConfig.loadAsync();
        }
    }
}
