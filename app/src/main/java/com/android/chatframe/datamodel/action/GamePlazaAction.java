package com.android.chatframe.datamodel.action;

import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;

import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DatabaseWrapper;
import com.android.chatframe.game.GameNativeInterface;
import com.android.chatframe.util.LogUtil;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-10-13.
 */

public class GamePlazaAction  extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;
    // Core parameters needed for all types of message
    private static final String KEY_IP = "room-ip";
    private static final String KEY_PORT = "room-port";
    private static final String KEY_INDEX = "room-index";

    public static boolean queueForLogonRoomInBackground(final String ip,final int port,final int roomIndex) {
        final GamePlazaAction action = new GamePlazaAction();
        return action.queueAction(ip,port,roomIndex);
    }

    /**
     * Constructor used for retrying dialing in the background (only message id available)
     */
    private GamePlazaAction() {
        super();
    }

    private boolean queueAction(final String ip,final int port,final int roomIndex) {
        actionParameters.putString(KEY_IP, ip);
        actionParameters.putInt(KEY_PORT,port);
        actionParameters.putInt(KEY_INDEX,roomIndex);
        LogUtil.i(TAG,
                "======GamePlazaAction===1===zhongjihao===queueAction====ip:  "+ip+"   port: "+port+"  index: "+roomIndex);
        start();
        LogUtil.i(TAG,
                "======GamePlazaAction===2===zhongjihao===queueAction====ip:  "+ip+"   port: "+port+"  index: "+roomIndex);

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "GamePlazaAction: Queued " + " for Game");
        }
        return true;
    }

    @Override
    protected Object executeAction() {
        LogUtil.i(TAG,
                "======GamePlazaAction===zhongjihao===executeAction====");
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
        return null;
    }

    @Override
    protected Bundle doBackgroundWork() {
        LogUtil.i(TAG, "===GamePlazaAction===zhongjihao===doBackgroundWork===");
        final String ip = actionParameters.getString(KEY_IP);
        final int port = actionParameters.getInt(KEY_PORT);
        final int roomIndex = actionParameters.getInt(KEY_INDEX);

        GameNativeInterface.connectGameRoom(ip,port,roomIndex);
        GameNativeInterface.loginGameRoom();
        return null;
    }

    @Override
    protected Object processBackgroundResponse(final Bundle response) {
        LogUtil.i(TAG,
                "======GamePlazaAction===zhongjihao====processBackgroundResponse====");
        // Nothing to do here
        return null;
    }

    @Override
    protected Object processBackgroundFailure() {
        LogUtil.i(TAG,
                "======GamePlazaAction===zhongjihao====processBackgroundFailure====");
        // Whether we succeeded or failed we will check and maybe schedule some more work
        return null;
    }

    private GamePlazaAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<GamePlazaAction> CREATOR
            = new Parcelable.Creator<GamePlazaAction>() {
        @Override
        public GamePlazaAction createFromParcel(final Parcel in) {
            return new GamePlazaAction(in);
        }

        @Override
        public GamePlazaAction[] newArray(final int size) {
            return new GamePlazaAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
