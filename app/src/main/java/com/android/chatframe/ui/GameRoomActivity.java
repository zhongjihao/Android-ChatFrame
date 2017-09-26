package com.android.chatframe.ui;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.KeyEvent;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.android.chatframe.BugleApplication;
import com.android.chatframe.R;
import com.android.chatframe.game.GameNativeInterface;
import com.android.chatframe.game.OcxGameCallbackImpl;
import com.android.chatframe.game.gametype.CMD_GR_ServerInfo;
import com.android.chatframe.game.gametype.CMD_GR_TableInfo;
import com.android.chatframe.game.gametype.CMD_GR_TableStatus;
import com.android.chatframe.game.gametype.CMD_GR_UserStatus;
import com.android.chatframe.game.gametype.GameConstant;
import com.android.chatframe.game.gametype.tagUserData;
import com.android.chatframe.util.Assert;
import com.squareup.leakcanary.RefWatcher;

import java.lang.ref.WeakReference;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-5.
 */

public class GameRoomActivity extends AppCompatActivity implements View.OnClickListener,OcxGameCallbackImpl.OcxRoomMsgEventListener {
    private TextView table_tv;
    private TextView chair_tv;
    private TextView online_tv;
    private Button onExit_btn;
    private Button onSitDown_btn;
    private Button onStandUp_btn;
    private Context context;
    private tagUserData mTagUserData;
    private MainHandler handle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gameroom);
        context = this;
        Bundle b = getIntent().getBundleExtra(UIIntents.LOGIN_FINISH_INFO);
        Assert.notNull(b);
        initView(b);
        handle = new MainHandler(this);
        OcxGameCallbackImpl.registerOcxRoomMsgEventListener(this);
    }

    private void initView(Bundle b){
        table_tv = (TextView)findViewById(R.id.table_tv);
        chair_tv = (TextView)findViewById(R.id.chair_tv);
        online_tv = (TextView)findViewById(R.id.online_tv);
        onExit_btn = (Button)findViewById(R.id.onExit_btn);
        onSitDown_btn = (Button)findViewById(R.id.onSitDown_btn);
        onStandUp_btn = (Button)findViewById(R.id.onStandUp_btn);

        onExit_btn.setOnClickListener(this);
        onSitDown_btn.setOnClickListener(this);
        onStandUp_btn.setOnClickListener(this);

        mTagUserData = b.getParcelable(UIIntents.UI_INTENT_EXTRA_TAG_USER_DATA);
        CMD_GR_ServerInfo serverInfo = b.getParcelable(UIIntents.UI_INTENT_EXTRA_SERVER_INFO_DATA);
        int onLineCount = b.getInt(UIIntents.UI_INTENT_EXTRA_SERVER_ONLINE_COUNT_DATA);
        table_tv.setText("桌子数: "+serverInfo.getwTableCount());
        chair_tv.setText("椅子数: "+serverInfo.getwChairCount());
        online_tv.setText("在线人数: "+onLineCount);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        GameNativeInterface.deleteRoom();
        RefWatcher refWatcher = BugleApplication.getRefWatcher(this);
        refWatcher.watch(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.onSitDown_btn:
                GameNativeInterface.sitDownReq((short)1,(short)1);
                break;
            case R.id.onStandUp_btn:
                GameNativeInterface.standUpReq((short)1,(short)1);
                break;
            case R.id.onExit_btn:
                GameNativeInterface.exitRoomReq((short)1,(short)1);
                break;
        }
    }

    @Override
    public void OnSocketSubUserStatus(CMD_GR_UserStatus userStatus) {
        if(mTagUserData.getDwUserID() == userStatus.getDwUserID()){
            mTagUserData.setwTableID(userStatus.getwTableID());
            mTagUserData.setwChairID(userStatus.getwChairID());
            mTagUserData.setCbUserStatus(userStatus.getCbUserStatus());
            switch (userStatus.getCbUserStatus()){
                case GameConstant.UserStatus.US_NULL:
                    finish();
                    break;
                case GameConstant.UserStatus.US_FREE:
                    Message.obtain(handle,1,"用户站立").sendToTarget();
                    break;
                case GameConstant.UserStatus.US_SIT:
                    Message.obtain(handle,1,"用户坐下").sendToTarget();
                    break;
            }
        }
    }

    @Override
    public void OnSocketMainTableStatus(CMD_GR_TableStatus tableStatus) {

    }

    @Override
    public void OnSocketMainTableInfo(CMD_GR_TableInfo tableInfo) {

    }

    static class MainHandler extends Handler {
        WeakReference<GameRoomActivity> actRef;

        MainHandler(GameRoomActivity activity) {
            actRef = new WeakReference<GameRoomActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            final GameRoomActivity act = actRef.get();
            if (act != null) {
                switch (msg.what){
                    case 1:
                        Toast.makeText(act,(String)(msg.obj),Toast.LENGTH_LONG).show();
                        break;
                }
            }

        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        return false;
    }
}
