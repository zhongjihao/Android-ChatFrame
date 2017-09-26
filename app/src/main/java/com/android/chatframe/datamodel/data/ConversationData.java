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

package com.android.chatframe.datamodel.data;

import android.app.LoaderManager;
import android.content.Loader;
import android.database.Cursor;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import com.android.chatframe.datamodel.action.DeleteMessageAction;
import com.android.chatframe.datamodel.binding.BindableData;
import com.android.chatframe.datamodel.binding.BindingBase;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.BoundCursorLoader;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.LogUtil;


public class ConversationData extends BindableData {

    private static final String TAG = "bugle_datamodel";
    private static final String BINDING_ID = "bindingId";
    private static final long LAST_MESSAGE_TIMESTAMP_NaN = -1;
    private static final int MESSAGE_COUNT_NaN = -1;
    private static final int CONVERSATION_MESSAGES_LOADER = 1;
    private final Context mContext;
    private final String mConversationId;
    private LoaderManager mLoaderManager;
    private final MessagesLoaderCallbacks mMessagesLoaderCallbacks;
    private long mLastMessageTimestamp = LAST_MESSAGE_TIMESTAMP_NaN;
    private int mMessageCount = MESSAGE_COUNT_NaN;


    @Override
    protected void unregisterListeners() {
        // This could be null if we bind but the caller doesn't init the BindableData
        if (mLoaderManager != null) {
            mLoaderManager.destroyLoader(CONVERSATION_MESSAGES_LOADER);
            mLoaderManager = null;
        }
    }

    public ConversationData(final Context context, final String conversationId) {
        Assert.isTrue(conversationId != null);
        mContext = context;
        mConversationId = conversationId;
        mMessagesLoaderCallbacks = new MessagesLoaderCallbacks();
    }

    public void setFocus() {
        DataModel.get().setFocusedConversation(mConversationId);
        // As we are loading the conversation assume the user has read the messages...
        // Do this late though so that it doesn't get in the way of other actions
        //TODO ...
    }

    public void unsetFocus() {
        DataModel.get().setFocusedConversation(null);
    }

    public boolean isFocused() {
        return isBound() && DataModel.get().isFocusedConversation(mConversationId);
    }

    public void init(final LoaderManager loaderManager,
                     final BindingBase<ConversationData> binding) {
        // Remember the binding id so that loader callbacks can check if data is still bound
        // to same ui component
        final Bundle args = new Bundle();
        args.putString(BINDING_ID, binding.getBindingId());
        mLoaderManager = loaderManager;
        mLoaderManager.initLoader(CONVERSATION_MESSAGES_LOADER, args, mMessagesLoaderCallbacks);
    }

    /**
     * A trampoline class so that we can inherit from LoaderManager.LoaderCallbacks multiple times.
     */
    private class MessagesLoaderCallbacks implements LoaderManager.LoaderCallbacks<Cursor> {
        @Override
        public Loader<Cursor> onCreateLoader(final int id, final Bundle args) {
            Assert.equals(CONVERSATION_MESSAGES_LOADER, id);
            Loader<Cursor> loader = null;

            final String bindingId = args.getString(BINDING_ID);
            // Check if data still bound to the requesting ui element
            if (isBound(bindingId)) {
                final Uri uri = Uri.parse("content://zhongjihao.test");
                loader = new BoundCursorLoader(bindingId, mContext, uri,
                        null, null, null, null);
                mLastMessageTimestamp = LAST_MESSAGE_TIMESTAMP_NaN;
                mMessageCount = MESSAGE_COUNT_NaN;
            } else {
                LogUtil.w(TAG, "Creating messages loader after unbinding mConversationId = " +
                        mConversationId);
            }
            return loader;
        }

        @Override
        public void onLoadFinished(final Loader<Cursor> generic, final Cursor rawData) {
            final BoundCursorLoader loader = (BoundCursorLoader) generic;

            // Check if data still bound to the requesting ui element
            if (isBound(loader.getBindingId())) {
                // Check if we have a new message, or if we had a message sync.
                boolean isSync = false;
                Cursor data = null;
                if (rawData != null) {
                   //TODO ...

                } else {
                    mMessageCount = MESSAGE_COUNT_NaN;
                }
            } else {
                LogUtil.w(TAG, "Messages loader finished after unbinding mConversationId = " +
                        mConversationId);
            }
        }

        @Override
        public void onLoaderReset(final Loader<Cursor> generic) {
            final BoundCursorLoader loader = (BoundCursorLoader) generic;

            // Check if data still bound to the requesting ui element
            if (isBound(loader.getBindingId())) {
                //TODO ...

                mLastMessageTimestamp = LAST_MESSAGE_TIMESTAMP_NaN;
                mMessageCount = MESSAGE_COUNT_NaN;
            } else {
                LogUtil.w(TAG, "Messages loader reset after unbinding mConversationId = " +
                        mConversationId);
            }
        }
    }

    public void sendMessage(final BindingBase<ConversationData> binding,
            final String message) {
        Assert.isTrue(binding.getData() == this);
        //TODO ...
       
    }

    public void resendMessage(final BindingBase<ConversationData> binding, final String messageId) {
        Assert.isTrue(binding.getData() == this);
        Assert.notNull(messageId);
    }

    public void deleteMessage(final BindingBase<ConversationData> binding, final String messageId) {
        Assert.isTrue(binding.getData() == this);
        Assert.notNull(messageId);
        DeleteMessageAction.deleteMessage(messageId);
    }

}
