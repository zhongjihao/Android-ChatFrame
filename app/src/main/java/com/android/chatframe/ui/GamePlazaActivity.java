package com.android.chatframe.ui;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Parcelable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Gravity;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.content.Context;

import com.android.chatframe.R;
import com.android.chatframe.game.GameNativeInterface;
import com.android.chatframe.game.OcxGameCallbackImpl;
import com.android.chatframe.game.OcxGameCallbackImpl.OcxLoginRoomEventListener;
import com.android.chatframe.game.gametype.CMD_GR_ColumnInfo;
import com.android.chatframe.game.gametype.CMD_GR_ServerInfo;
import com.android.chatframe.game.gametype.tagGameServer;
import com.android.chatframe.game.gametype.CMD_GP_LogonSuccess;
import com.android.chatframe.game.gametype.tagUserData;
import com.android.chatframe.game.gametype.GameConstant;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.LogUtil;

import java.lang.ref.WeakReference;


/**
 * Created by SPREADTRUM\jihao.zhong on 17-8-4.
 */

public class GamePlazaActivity extends AppCompatActivity implements OcxLoginRoomEventListener{

    private TextView accInfo_tv;
    private TextView title_tv;
    private LinearLayout room_llayout;
    private Context context;
    private MainHandler handle;
    private CMD_GR_ServerInfo mServerInfo;
    private int mOnlineCount;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gameplaza);
        context = this;
        accInfo_tv = (TextView)findViewById(R.id.accInfo_tv);
        title_tv = (TextView)findViewById(R.id.title_tv);
        room_llayout = (LinearLayout)findViewById(R.id.room_llayout);
        Bundle b = getIntent().getBundleExtra(UIIntents.LOGIN_FINISH_INFO);
        Assert.notNull(b);
        initView(b);

        handle = new MainHandler(this);
        OcxGameCallbackImpl.registerOcxLoginRoomEventListener(this);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    private void initView(Bundle b){
        title_tv.setText(b.getString(UIIntents.UI_INTENT_EXTRA_PLAZA_NAME_DATA));

        CMD_GP_LogonSuccess logonSuccess = b.getParcelable(UIIntents.UI_INTENT_EXTRA_LOGIN_PLAZA_SUCCESS_DATA);

        accInfo_tv.setText("性别： "+(logonSuccess.getCbGender() == GameConstant.UserGender.GENDER_BOY ? 'M' : 'F')
                          + "  \n会员等级 : "+logonSuccess.getCbMember()+"  \n用户经验: "
                          +logonSuccess.getDwExperience()+"  \n用户魅力: "+logonSuccess.getlLoveliness()+"   \n用户金币: "
                          +logonSuccess.getlGoldCoin()+"  \n用户积分: "+logonSuccess.getlScore());
        final Parcelable[] gameServer = b.getParcelableArray(UIIntents.UI_INTENT_EXTRA_GAMEROOM_LIST_DATA);
        for(int i=0;i <gameServer.length;i++){
            Button roomBtn = new Button(this);
            final tagGameServer gameRoom = (tagGameServer)gameServer[i];
            roomBtn.setText(gameRoom.getSzServerName() + "   \n房间人数 : "+ gameRoom.getDwOnLineCount());
            roomBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int room_index = (int)v.getTag();
                    LogUtil.d("MainActivity", "===GameClient===房间地址: " +gameRoom.getDwServerAddr()+" 房间端口: "+gameRoom.getwServerPort()+"  房间索引: "+room_index);
                    Toast.makeText(context,"房间地址: "+gameRoom.getDwServerAddr()+" 房间端口: "+gameRoom.getwServerPort(),Toast.LENGTH_LONG).show();
                    GameNativeInterface.connectGameRoom(gameRoom.getDwServerAddr(),gameRoom.getwServerPort(),room_index);
                    GameNativeInterface.loginGameRoom();
                    //收到用户进入消息后，启动房间界面
                }
            });

            roomBtn.setGravity(Gravity.CENTER_HORIZONTAL);
            LinearLayout.LayoutParams linearParams = new LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.WRAP_CONTENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
            );
            linearParams.gravity =  Gravity.CENTER_HORIZONTAL;
            linearParams.topMargin = 15;
            roomBtn.setLayoutParams(linearParams);
            roomBtn.setTag(i);
            room_llayout.addView(roomBtn);
        }
    }

    @Override
    public void OnSocketMainLogonFail(int errorCode, String errorDescribe) {
        Message.obtain(handle,1,errorDescribe).sendToTarget();
    }

    @Override
    public void OnSocketMainLoginFinish(CMD_GR_ColumnInfo columnInfo, CMD_GR_ServerInfo serverInfo, int onlineCount) {
        mServerInfo = serverInfo;
        mOnlineCount = onlineCount;
    }

    @Override
    public void OnSocketSubUserCome(tagUserData userData, String pUnderWrite, String pGroupName) {
        if(mServerInfo != null){
            Bundle b = new Bundle();
            b.putParcelable(UIIntents.UI_INTENT_EXTRA_TAG_USER_DATA,userData);
            b.putParcelable(UIIntents.UI_INTENT_EXTRA_SERVER_INFO_DATA,mServerInfo);
            b.putInt(UIIntents.UI_INTENT_EXTRA_SERVER_ONLINE_COUNT_DATA,mOnlineCount);
            UIIntents.get().launchGameRoomActivity(this,b);
            mServerInfo = null;
        }

    }

    static class MainHandler extends Handler {
        WeakReference<GamePlazaActivity> actRef;

        MainHandler(GamePlazaActivity activity) {
            actRef = new WeakReference<GamePlazaActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            final GamePlazaActivity act = actRef.get();
            if (act != null) {
                switch (msg.what){
                    case 1:
                        Toast.makeText(act,(String)(msg.obj),Toast.LENGTH_LONG).show();
                        break;
                }
            }

        }
    }
}
