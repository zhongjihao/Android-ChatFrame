package com.android.chatframe.datamodel.action;

import android.os.Bundle;
import android.os.Parcel;
import android.os.Parcelable;

import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.DatabaseWrapper;
import com.android.chatframe.game.GameNativeInterface;
import com.android.chatframe.util.LogUtil;

import java.util.ArrayList;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-10-11.
 */

public class GameLogonAction extends Action implements Parcelable {
    private static final String TAG = LogUtil.BUGLE_DATAMODEL_TAG;
    // Core parameters needed for all types of message
    private static final String KEY_TYPE = "action-type";
    private static final String KEY_GAME_PLAZA = "game-plaza";

    public final static int ONLOGONPLAZA = 1;
    public final static int ONREGISTERPLAZA = 2;
    public final static int ONUNREGISTERPLAZA = 3;

    public static boolean queueForDialInBackground(final int type,final ArrayList<String> logonInfo) {
        LogUtil.i(TAG,
                "======GameLogonAction===zhongjihao===queueForSendInBackground====type: "+type);
        final GameLogonAction action = new GameLogonAction();
        return action.queueAction(type,logonInfo);
    }

    /**
     * Constructor used for retrying dialing in the background (only message id available)
     */
    private GameLogonAction() {
        super();
    }

    private boolean queueAction(final int type,final ArrayList<String> logonInfo) {
        actionParameters.putInt(KEY_TYPE, type);
        actionParameters.putStringArrayList(KEY_GAME_PLAZA,logonInfo);
        LogUtil.i(TAG,
                "======GameLogonAction===1===zhongjihao===queueAction====type:  "+type);
        start();
        LogUtil.i(TAG,
                "======GameLogonAction===2===zhongjihao===queueAction====type:  "+type);

        if (LogUtil.isLoggable(TAG, LogUtil.DEBUG)) {
            LogUtil.d(TAG, "GameLogonAction: Queued " + " for Game");
        }
        return true;
    }

    @Override
    protected Object executeAction() {
        LogUtil.i(TAG,
                "======GameLogonAction===zhongjihao===executeAction====");
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
        LogUtil.i(TAG, "===GameLogonAction===zhongjihao===doBackgroundWork===");
        final int type = actionParameters.getInt(KEY_TYPE);
        final ArrayList<String> data = actionParameters.getStringArrayList(KEY_GAME_PLAZA);

        switch (type){
            case ONLOGONPLAZA:       //登陆大厅
                GameNativeInterface.connectGamePlaza("127.0.0.1",8888);
                GameNativeInterface.loginGamePlaza(data.get(0),data.get(1),Integer.valueOf(data.get(2)));
                break;
            case ONREGISTERPLAZA:       //注册大厅账号
                GameNativeInterface.connectGamePlaza("127.0.0.1",8888);
                GameNativeInterface.registerGamePlaza(data.get(0),data.get(1),data.get(2));
                break;
            case ONUNREGISTERPLAZA:       //注销大厅账号
                GameNativeInterface.connectGamePlaza("127.0.0.1",8888);
                GameNativeInterface.unregisterGameAccount(Integer.valueOf(data.get(0)));
                break;
        }
        return null;
    }

    @Override
    protected Object processBackgroundResponse(final Bundle response) {
        LogUtil.i(TAG,
                "======GameLogonAction===zhongjihao====processBackgroundResponse====");
        // Nothing to do here
        return null;
    }

    @Override
    protected Object processBackgroundFailure() {
        LogUtil.i(TAG,
                "======GameLogonAction===zhongjihao====processBackgroundFailure====");
        // Whether we succeeded or failed we will check and maybe schedule some more work
        return null;
    }

    private GameLogonAction(final Parcel in) {
        super(in);
    }

    public static final Parcelable.Creator<GameLogonAction> CREATOR
            = new Parcelable.Creator<GameLogonAction>() {
        @Override
        public GameLogonAction createFromParcel(final Parcel in) {
            return new GameLogonAction(in);
        }

        @Override
        public GameLogonAction[] newArray(final int size) {
            return new GameLogonAction[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel parcel, final int flags) {
        writeActionToParcel(parcel, flags);
    }
}
