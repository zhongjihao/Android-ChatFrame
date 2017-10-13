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

public class GameRoomAction  extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;
    // Core parameters needed for all types of message
    private static final String KEY_CMD = "room-cmd";
    private static final String KEY_TABLE = "room-table";
    private static final String KEY_CHAIR = "room-chair";

    public final static int SIT_DOWN_REQ = 1;
    public final static int STAND_UP_REQ = 2;
    public final static int EXIT_ROOM_REQ = 3;

    public static boolean queueForRoomRequestInBackground(final int cmdId,final short tableId,final short chairId) {
        final GameRoomAction action = new GameRoomAction();
        return action.queueAction(cmdId,tableId,chairId);
    }

    /**
     * Constructor used for retrying dialing in the background (only message id available)
     */
    private GameRoomAction() {
        super();
    }

    private boolean queueAction(final int cmdId,final short tableId,final short chairId) {
        actionParameters.putInt(KEY_CMD,cmdId);
        actionParameters.putShort(KEY_TABLE,tableId);
        actionParameters.putShort(KEY_CHAIR,chairId);
        LogUtil.i(TAG,
                "======GameRoomAction======zhongjihao===queueAction====tableId:  "+tableId+"   chairId: "+chairId);
        start();

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "GamePlazaAction: Queued " + " for Game");
        }
        return true;
    }

    @Override
    protected Object executeAction() {
        LogUtil.i(TAG,
                "======GameRoomAction===zhongjihao===executeAction====");
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
        final int cmdId = actionParameters.getInt(KEY_CMD);
        final short tableId = actionParameters.getShort(KEY_TABLE);
        final short chairId = actionParameters.getShort(KEY_CHAIR);
        LogUtil.i(TAG,
                "===GameRoomAction======zhongjihao===doBackgroundWork====tableId:  "+tableId+"   chairId: "+chairId+"   cmdId: "+cmdId);
        switch (cmdId){
            case SIT_DOWN_REQ:
                GameNativeInterface.sitDownReq(tableId,chairId);
                break;
            case STAND_UP_REQ:
                GameNativeInterface.standUpReq(tableId,chairId);
                break;
            case EXIT_ROOM_REQ:
                GameNativeInterface.exitRoomReq(tableId,chairId);
                break;
        }
        return null;
    }

    @Override
    protected Object processBackgroundResponse(final Bundle response) {
        LogUtil.i(TAG,
                "======GameRoomAction===zhongjihao====processBackgroundResponse====");
        // Nothing to do here
        return null;
    }

    @Override
    protected Object processBackgroundFailure() {
        LogUtil.i(TAG,
                "======GameRoomAction===zhongjihao====processBackgroundFailure====");
        // Whether we succeeded or failed we will check and maybe schedule some more work
        return null;
    }

    private GameRoomAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<GameRoomAction> CREATOR
            = new Parcelable.Creator<GameRoomAction>() {
        @Override
        public GameRoomAction createFromParcel(final Parcel in) {
            return new GameRoomAction(in);
        }

        @Override
        public GameRoomAction[] newArray(final int size) {
            return new GameRoomAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
