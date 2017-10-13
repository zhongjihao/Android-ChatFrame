package com.android.chatframe.datamodel.action;

import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;

import com.android.chatframe.BugleApplication;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DatabaseWrapper;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.Assert.RunsOnMainThread;
import com.android.chatframe.util.LogUtil;

import java.util.ArrayList;

/**
 * Action used to send an outgoing message. It writes MMS messages to the telephony db
 * ({@link InsertNewMessageAction}) writes SMS messages to the telephony db). It also
 * initiates the actual sending. It will all be used for re-sending a failed message.
 * NOTE: This action must queue a ProcessPendingMessagesAction when it is done (success or failure).
 * <p>
 * This class is public (not package-private) because the SMS/MMS (e.g. MmsUtils) classes need to
 * access the EXTRA_* fields for setting up the 'sent' pending intent.
 */
public class SendMessageAction extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;
    // Core parameters needed for all types of message
    private static final String KEY_MESSAGE_ID = "number";
    private static final String KEY_MESSAGE = "message";

    /**
     * Interface for SendMessageAction listeners
     */
    public interface SendMessageActionListener {
        @RunsOnMainThread
        abstract void onSendMessageSucceeded(final ActionMonitor monitor,
                                                         final Object data, final String result);

        @RunsOnMainThread
        abstract void onSendMessageFailed(final ActionMonitor monitor, final Object data);
    }

    public static boolean queueForSendInBackground(final String dest,final String content) {
        LogUtil.i(TAG,
                "======SendMessageAction===zhongjihao===queueForSendInBackground====messageId: "+dest+"    content: "+content);
        final SendMessageAction action = new SendMessageAction();
        return action.queueAction(dest,content);
    }

    public static SendMessageAction.SendMessageActionMonitor getOrCreateSendMessageMonitor(
            final ArrayList<String> participants, final Object data,
            final SendMessageAction.SendMessageActionListener listener) {
        final SendMessageAction.SendMessageActionMonitor monitor = new
                SendMessageAction.SendMessageActionMonitor(data, listener);
        final SendMessageAction action = new SendMessageAction(participants,
                monitor.getActionKey());
        action.start(monitor);
        return monitor;
    }

    /**
     * Constructor used for retrying sending in the background (only message id available)
     */
    private SendMessageAction() {
        super();
    }

    private static final String KEY_PARTICIPANTS_LIST = "participants_list";

    private SendMessageAction(final ArrayList<String> participants,final String actionKey) {
        super(actionKey);
        actionParameters.putStringArrayList(KEY_PARTICIPANTS_LIST,participants);
    }

    /**
     * Read message from database and queue actual sending
     */
    private boolean queueAction(final String dest,final String content) {
        actionParameters.putString(KEY_MESSAGE_ID, dest);
        actionParameters.putString(KEY_MESSAGE, content);

        LogUtil.i(TAG,
                "======SendMessageAction===1===zhongjihao===queueAction====messageId: "+dest+"    content: "+content);

        start();
        LogUtil.i(TAG,
                "======SendMessageAction===2===zhongjihao===queueAction====messageId: "+dest+"    content: "+content);

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "SendMessageAction: Queued MMS message " + dest
                    + " for sending");
        }
        return true;

    }

    @Override
    protected Object executeAction() {
        /*
        //方式一
        final String messageId = actionParameters.getString(KEY_MESSAGE_ID);
        final String message = actionParameters.getString(KEY_MESSAGE);
        LogUtil.i(TAG,
                "======SendMessageAction===zhongjihao===executeAction====phone: "+messageId+"   content: "+message);
        final DatabaseWrapper db = DataModel.get().getDatabase();

        // Write send message to our DB
        db.beginTransaction();
        try {
            // TODO ...

            db.setTransactionSuccessful();
        } finally {
            db.endTransaction();
        }

        requestBackgroundWork();
        */

        //方式二
        final ArrayList<String> data = actionParameters.getStringArrayList(KEY_PARTICIPANTS_LIST);
        LogUtil.i(TAG,
                "======SendMessageAction===zhongjihao===executeAction====phone: "+data.get(0)+"   cotent: "+data.get(1));
        final DatabaseWrapper db = DataModel.get().getDatabase();

        // Write send message to our DB
        db.beginTransaction();
        try {
            // TODO ...

            db.setTransactionSuccessful();
        } finally {
            db.endTransaction();
        }

        requestBackgroundWork();
        return "executeAction ok!!";
    }

    /**
     * Send message on background worker thread
     */
    @Override
    protected Bundle doBackgroundWork() {
        /*
        //方式一
        final String message = actionParameters.getString(KEY_MESSAGE);
        final String messageId = actionParameters.getString(KEY_MESSAGE_ID);
        Assert.notNull(messageId);
        BugleApplication.getDispatcher().sendSMS(messageId,message);
        */

        //方式二
        final ArrayList<String> data = actionParameters.getStringArrayList(KEY_PARTICIPANTS_LIST);
        Assert.notNull(data);
        Assert.notNull(data.get(0));
        LogUtil.i(TAG, "===SendMessageAction===zhongjihao===doBackgroundWork===phone: "
                + data.get(0) + "    content: "+data.get(1));

        BugleApplication.getDispatcher().sendSMS(data.get(0),data.get(1));
        Bundle b = new Bundle();
        b.putString("yanxue","徐延雪");
        b.putString("zhidongni","支东妮");
        return b;
    }

    @Override
    protected Object processBackgroundResponse(final Bundle response) {
        String s1 = (String)response.get("yanxue");
        String s2 = (String)response.get("zhidongni");
        LogUtil.i(TAG,
                "======SendMessageAction===zhongjihao====processBackgroundResponse====s1: "+s1+"   s2: "+s2);
        // Nothing to do here
        return "processBackgroundResponse执行了";
    }

    /**
     * Update message status to reflect success or failure
     */
    @Override
    protected Object processBackgroundFailure() {
        final String messageId = actionParameters.getString(KEY_MESSAGE_ID);
        LogUtil.i(TAG,
                "======SendMessageAction===zhongjihao====processBackgroundFailure====messageId: "+messageId);
        //TODO ...


        // Whether we succeeded or failed we will check and maybe schedule some more work


        return null;
    }


    /**
     * A monitor that notifies a listener upon completion
     */
    public static class SendMessageActionMonitor extends ActionMonitor
            implements ActionMonitor.ActionCompletedListener {
        private final SendMessageAction.SendMessageActionListener mListener;

        SendMessageActionMonitor(final Object data,
                                             final SendMessageAction.SendMessageActionListener listener) {
            super(STATE_CREATED, generateUniqueActionKey("SendMessageAction"), data);
            setCompletedListener(this);
            mListener = listener;
        }

        @Override
        public void onActionSucceeded(final ActionMonitor monitor,
                                      final Action action, final Object data, final Object result) {
            LogUtil.i(TAG,
                    "======SendMessageAction===zhongjihao====onActionSucceeded===data: "+data+"   result: "+result);
            if (result == null) {
                mListener.onSendMessageFailed(monitor, data);
            } else {
                mListener.onSendMessageSucceeded(monitor, data, (String) result);
            }
        }

        @Override
        public void onActionFailed(final ActionMonitor monitor,
                                   final Action action, final Object data, final Object result) {
            // TODO: Currently onActionFailed is only called if there is an error in
            // processing requests, not for errors in the local processing.
            LogUtil.i(TAG,
                    "======SendMessageAction===zhongjihao====onActionFailed===data: "+data+"   result: "+result);
            Assert.fail("Unreachable");
            mListener.onSendMessageFailed(monitor, data);
        }
    }



    private SendMessageAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<SendMessageAction> CREATOR
            = new Parcelable.Creator<SendMessageAction>() {
        @Override
        public SendMessageAction createFromParcel(final Parcel in) {
            return new SendMessageAction(in);
        }

        @Override
        public SendMessageAction[] newArray(final int size) {
            return new SendMessageAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
