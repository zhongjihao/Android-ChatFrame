package com.android.chatframe.datamodel.action;

import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;

import com.android.chatframe.BugleApplication;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DatabaseWrapper;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.LogUtil;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-6-4.
 */

public class DialAction extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;
    // Core parameters needed for all types of message
    private static final String KEY_TYPE = "action-type";

    /**
     * Interface for DialAction listeners
     */
    public interface DialActionListener {
        @Assert.RunsOnMainThread
        abstract void onDialSucceeded(final ActionMonitor monitor,
                                             final Object data, final String result);

        @Assert.RunsOnMainThread
        abstract void onDialFailed(final ActionMonitor monitor, final Object data);
    }

    public static boolean queueForDialInBackground(final int type) {
        LogUtil.i(TAG,
                "======DilaAction===zhongjihao===queueForSendInBackground====type: "+type);
        final DialAction action = new DialAction();
        return action.queueAction(type);
    }

    public static DialAction.DialActionMonitor getOrCreateDialMonitor(final int type,final Object data,
            final DialAction.DialActionListener listener) {
        final DialAction.DialActionMonitor monitor = new
                DialAction.DialActionMonitor(data, listener);
        final DialAction action = new DialAction(type,
                monitor.getActionKey());
        action.start(monitor);
        return monitor;
    }

    /**
     * Constructor used for retrying dialing in the background (only message id available)
     */
    private DialAction() {
        super();
    }

    private DialAction(final int type,final String actionKey) {
        super(actionKey);
        actionParameters.putInt(KEY_TYPE, type);
    }


    private boolean queueAction(final int type) {
        actionParameters.putInt(KEY_TYPE, type);
        LogUtil.i(TAG,
                "======DialAction===1===zhongjihao===queueAction====type:  "+type);
        start();
        LogUtil.i(TAG,
                "======DialAction===2===zhongjihao===queueAction====type:  "+type);

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "DialAction: Queued " + " for Dial");
        }
        return true;
    }

    @Override
    protected Object executeAction() {
        final int type = actionParameters.getInt(KEY_TYPE);

        LogUtil.i(TAG,
                "======DialAction===zhongjihao===executeAction====type: "+type);
        final DatabaseWrapper db = DataModel.get().getDatabase();

        // Write send message to our DB
        db.beginTransaction();
        try {
            // TODO ...

            db.setTransactionSuccessful();
        } finally {
            db.endTransaction();
        }
        String ret = null;
        switch (type){
            case 1:       //dial
                BugleApplication.getDispatcher().dial("18626455927",100);
                ret = "DIAL executeAction ok!!";
                break;
            case 2:       //accept
                BugleApplication.getDispatcher().acceptCall();
                ret = "ACCEPT executeAction ok!!";
                break;
            case 3:       //reject
                BugleApplication.getDispatcher().rejectCall();
                ret = "REJECT executeAction ok!!";
                break;
            case 4:       //hangup
                BugleApplication.getDispatcher().hangupConnection(2);
                ret = "HANGUP executeAction ok!!";
                break;
        }

        return ret;
    }

    @Override
    protected Bundle doBackgroundWork() {
        LogUtil.i(TAG, "===DialAction===zhongjihao===doBackgroundWork===");
        return null;
    }

    @Override
    protected Object processBackgroundResponse(final Bundle response) {
        LogUtil.i(TAG,
                "======DialAction===zhongjihao====processBackgroundResponse====");
        // Nothing to do here
        return null;
    }

    @Override
    protected Object processBackgroundFailure() {
        LogUtil.i(TAG,
                "======DialAction===zhongjihao====processBackgroundFailure====");
        // Whether we succeeded or failed we will check and maybe schedule some more work
        return null;
    }

    /**
     * A monitor that notifies a listener upon completion
     */
    public static class DialActionMonitor extends ActionMonitor
            implements ActionMonitor.ActionCompletedListener {
        private final DialAction.DialActionListener mListener;

        DialActionMonitor(final Object data,
                                 final DialAction.DialActionListener listener) {
            super(STATE_CREATED, generateUniqueActionKey("DialAction"), data);
            setCompletedListener(this);
            mListener = listener;
        }

        @Override
        public void onActionSucceeded(final ActionMonitor monitor,
                                      final Action action, final Object data, final Object result) {
            LogUtil.i(TAG,
                    "======DialAction===zhongjihao====onActionSucceeded===data: "+data+"   result: "+result);
            if (result == null) {
                mListener.onDialFailed(monitor, data);
            } else {
                mListener.onDialSucceeded(monitor, data, (String) result);
            }
        }

        @Override
        public void onActionFailed(final ActionMonitor monitor,
                                   final Action action, final Object data, final Object result) {
            // TODO: Currently onActionFailed is only called if there is an error in
            // processing requests, not for errors in the local processing.
            LogUtil.i(TAG,
                    "======DialAction===zhongjihao====onActionFailed===data: "+data+"   result: "+result);
            Assert.fail("Unreachable");
            mListener.onDialFailed(monitor, data);
        }
    }



    private DialAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<DialAction> CREATOR
            = new Parcelable.Creator<DialAction>() {
        @Override
        public DialAction createFromParcel(final Parcel in) {
            return new DialAction(in);
        }

        @Override
        public DialAction[] newArray(final int size) {
            return new DialAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
