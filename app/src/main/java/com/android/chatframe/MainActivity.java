package com.android.chatframe;

import android.database.Cursor;
import android.os.Message;
import android.os.Handler;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.SharedPreferences;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;

import com.android.chatframe.framework.MsgDispatcher;
import com.android.chatframe.framework.RequestEventListener;
import com.android.chatframe.datamodel.action.SendMessageAction;
import com.android.chatframe.datamodel.action.DialAction;
import com.android.chatframe.datamodel.action.GameLogonAction;
import com.android.chatframe.datamodel.action.DialAction.DialActionMonitor;
import com.android.chatframe.datamodel.action.SendMessageAction.SendMessageActionMonitor;
import com.android.chatframe.datamodel.action.ActionMonitor;
import com.android.chatframe.datamodel.DataModel;
import com.android.chatframe.datamodel.binding.BindingBase;
import com.android.chatframe.datamodel.binding.Binding;
import com.android.chatframe.datamodel.data.ConversationListData;
import com.android.chatframe.game.GameNativeInterface;
import com.android.chatframe.game.OcxGameCallback;
import com.android.chatframe.game.OcxGameCallbackImpl;
import com.android.chatframe.game.OcxGameCallbackImpl.OcxPlazaEventListener;
import com.android.chatframe.game.gametype.CMD_GP_LogonSuccess;
import com.android.chatframe.game.gametype.CMD_GP_RegisterSuccess;
import com.android.chatframe.game.gametype.tagGameKind;
import com.android.chatframe.game.gametype.tagGameServer;
import com.android.chatframe.game.gametype.tagGameStation;
import com.android.chatframe.game.gametype.tagGameType;
import com.android.chatframe.swig.SwigTest;
import com.android.chatframe.ui.UIIntents;
import com.android.chatframe.util.Assert;
import com.android.chatframe.util.Assert.RunsOnMainThread;
import com.android.chatframe.util.LogUtil;

import java.util.ArrayList;
import java.lang.ref.WeakReference;


public class MainActivity extends AppCompatActivity implements SendMessageAction.SendMessageActionListener, DialAction.DialActionListener,ConversationListData.ConversationListDataListener,View.OnClickListener,RequestEventListener,OcxPlazaEventListener{
    private Button smsBtn;
    private Button dialBtn;
    private Button acceptBtn;
    private Button hangUpBtn;
    private Button rejectBtn;
    private Button logonPlazaBtn;
    private Button registerPlazaBtn;
    private Button unregisterBtn;
    private TextView resultTxt;

    final Binding<ConversationListData> mListBinding = BindingBase.createBinding(this);

    private OcxGameCallback ocx;

    private CMD_GP_LogonSuccess mLogonSuccess = null;
    private tagGameServer[] mGameServer = null;
    private String mPlazaName = null;

    private MainHandler handle;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(SwigTest.SayHello());

        smsBtn = (Button)findViewById(R.id.sendSmsBtn);
        dialBtn = (Button)findViewById(R.id.dialBtn);
        acceptBtn = (Button)findViewById(R.id.acceptBtn);
        hangUpBtn = (Button)findViewById(R.id.hangUpBtn);
        rejectBtn = (Button)findViewById(R.id.rejectBtn);
        logonPlazaBtn = (Button)findViewById(R.id.logonPlazaBtn);
        registerPlazaBtn = (Button)findViewById(R.id.registerPlazaBtn);
        unregisterBtn = (Button)findViewById(R.id.unregisterBtn);
        resultTxt = (TextView)findViewById(R.id.resultTxt);
        smsBtn.setOnClickListener(this);
        dialBtn.setOnClickListener(this);
        acceptBtn.setOnClickListener(this);
        hangUpBtn.setOnClickListener(this);
        rejectBtn.setOnClickListener(this);
        logonPlazaBtn.setOnClickListener(this);
        registerPlazaBtn.setOnClickListener(this);
        unregisterBtn.setOnClickListener(this);

        mListBinding.bind(DataModel.get().createConversationListData(getApplication(), this, false));
        mListBinding.getData().init(getLoaderManager(), mListBinding);

        BugleApplication.getDispatcher().registerEventListener(this);

        ocx = new OcxGameCallbackImpl();
        GameNativeInterface.registerOcxInterface(ocx);
        OcxGameCallbackImpl.registerOcxPlazaEventListener(this);


        handle = new MainHandler(this);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (mListBinding.isBound()) {
            mListBinding.unbind();
        }
        if (mSendMsgMonitor != null) {
            mSendMsgMonitor.unregister();
        }
        mSendMsgMonitor = null;

        if (mDialMonitor != null) {
            mDialMonitor.unregister();
        }
        mDialMonitor = null;
    }

    private SendMessageActionMonitor mSendMsgMonitor;
    private DialActionMonitor mDialMonitor;

    @Override
    @RunsOnMainThread
    public void onSendMessageSucceeded(final ActionMonitor monitor,
                                       final Object data, final String result) {
        Assert.isTrue(monitor == mSendMsgMonitor);
        Assert.isTrue(result != null);
        LogUtil.d("MainActivity", "=====zhongjihao====1===onSendMessageSucceeded=========result: " + result);
        final ConversationListData bindingId = (ConversationListData) data;
        if (mListBinding.isBound(bindingId)){
            //Todo ...
            LogUtil.d("MainActivity", "=====zhongjihao====2===onSendMessageSucceeded=========result: " + result);
            Toast.makeText(this,result,Toast.LENGTH_LONG).show();
        }
        mSendMsgMonitor = null;
    }

    @Override
    @RunsOnMainThread
    public void onSendMessageFailed(final ActionMonitor monitor,
                                    final Object data) {
        Assert.isTrue(monitor == mSendMsgMonitor);
        LogUtil.e("MainActivity", "===1===zhongjihao=====onSendMessageFailed=======");
        final ConversationListData bindingId = (ConversationListData) data;
        if (mListBinding.isBound(bindingId)){
            //Todo ...
            LogUtil.e("MainActivity", "===2===zhongjihao=====onSendMessageFailed=======");
        }
        mSendMsgMonitor = null;
    }

    @Override
    @RunsOnMainThread
    public void onDialSucceeded(final ActionMonitor monitor, final Object data, final String result) {
        Assert.isTrue(monitor == mDialMonitor);
        Assert.isTrue(result != null);
        LogUtil.d("MainActivity", "=====zhongjihao====1===onDialSucceeded=========result: " + result);
        final ConversationListData bindingId = (ConversationListData) data;
        if (mListBinding.isBound(bindingId)){
            //Todo ...
            LogUtil.d("MainActivity", "=====zhongjihao====2===onDialSucceeded=========result: " + result);
            Toast.makeText(this,result,Toast.LENGTH_LONG).show();
        }
        mDialMonitor = null;
    }

    @Override
    @RunsOnMainThread
    public void onDialFailed(final ActionMonitor monitor, final Object data) {
        Assert.isTrue(monitor == mDialMonitor);
        LogUtil.e("MainActivity", "===1===zhongjihao=====onDialFailed=======");
        final ConversationListData bindingId = (ConversationListData) data;
        if (mListBinding.isBound(bindingId)){
            //Todo ...
            LogUtil.e("MainActivity", "===2===zhongjihao=====onDialFailed=======");
            Toast.makeText(this,"DialAction Failed!!!",Toast.LENGTH_LONG).show();
        }
        mDialMonitor = null;
    }

    public void onConversationListCursorUpdated(ConversationListData data, Cursor cursor){
        LogUtil.e("MainActivity", "======zhongjihao=====onConversationListCursorUpdated=======");
    }

    public void setBlockedParticipantsAvailable(boolean blockedAvailable){
        LogUtil.e("MainActivity", "======zhongjihao=====setBlockedParticipantsAvailable=======blockedAvailable:"+blockedAvailable);
    }

    @Override
    public void onSentSmsComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onDialComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onAcceptComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onHangupComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onRejectComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onNewMessageArrive(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onConnectComplete(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onIncoming(Message msg) {
        handleEventComplete(msg);
    }

    @Override
    public void onClick(View v) {
        int vId = v.getId();
        switch (vId){
            case R.id.sendSmsBtn:
                /*
                //方式一启动action
                 SendMessageAction.queueForSendInBackground("18626455927","android开发大师你好!");
               */

                //方式二启动acion
                if (mListBinding.isBound() && mSendMsgMonitor == null) {
                    ArrayList<String> data = new ArrayList<String>();
                    data.add("18626455927");
                    data.add("android开发大师你好!");
                    mSendMsgMonitor = SendMessageAction.getOrCreateSendMessageMonitor(data, mListBinding.getData(), this);
                }
                break;
            case R.id.dialBtn:
                if (mListBinding.isBound() && mDialMonitor == null) {
                    mDialMonitor = DialAction.getOrCreateDialMonitor(1,mListBinding.getData(), this);
                }
                break;
            case R.id.acceptBtn:
                DialAction.queueForDialInBackground(2);
                break;
            case R.id.hangUpBtn:
                DialAction.queueForDialInBackground(4);
                break;
            case R.id.rejectBtn:
                if (mListBinding.isBound() && mDialMonitor == null) {
                    mDialMonitor = DialAction.getOrCreateDialMonitor(3,mListBinding.getData(), this);
                }
                break;
            case R.id.logonPlazaBtn: {
                LogUtil.d("MainActivity", "=====GameClient=====OnClick   logonPlazaBtn==" );
                SharedPreferences sp = getSharedPreferences("acccountBaseInfo", Context.MODE_PRIVATE);
                String account = sp.getString("account", "zhidongni");
                String pwd = sp.getString("pwd", "10010");
                int userId = sp.getInt("userId", 0);
                ArrayList<String> data = new ArrayList<String>();
                data.add(account);
                data.add(pwd);
                data.add(userId+"");
                GameLogonAction.queueForDialInBackground(GameLogonAction.ONLOGONPLAZA,data);
                break;
            }
            case R.id.registerPlazaBtn:{
                LogUtil.d("MainActivity", "=====GameClient=====OnClick   registerPlazaBtn==" );
                ArrayList<String> data = new ArrayList<String>();
                data.add("zhidongni");
                data.add("10010");
                data.add("18626455927");
                GameLogonAction.queueForDialInBackground(GameLogonAction.ONREGISTERPLAZA,data);
                break;
            }
            case R.id.unregisterBtn:{
                LogUtil.d("MainActivity", "=====GameClient=====OnClick   unregisterBtn==" );
                SharedPreferences rsp = getSharedPreferences("acccountBaseInfo", Context.MODE_PRIVATE);
                int user_Id = rsp.getInt("userId", 0);
                ArrayList<String> data = new ArrayList<String>();
                data.add(user_Id+"");
                GameLogonAction.queueForDialInBackground(GameLogonAction.ONUNREGISTERPLAZA,data);
                break;
            }
        }
    }

    private void handleEventComplete(Message msg){
        switch (msg.what) {
            case MsgDispatcher.EVENT_SEND_SMS_COMPLETE:
            case MsgDispatcher.EVENT_NEW_SMS:
            case MsgDispatcher.EVENT_DIAL_COMPLETE:
            case MsgDispatcher.EVENT_ACCEPT_CALL_COMPLETE:
            case MsgDispatcher.EVENT_HANGUP_COMPLETE:
            case MsgDispatcher.EVENT_REJECT_CALL_COMPLETE:
            case MsgDispatcher.EVENT_CALL_RING:
                if(msg.getData() != null && msg.getData().getSerializable("Throwable") !=null){
                    Toast.makeText(this,(String)(msg.obj),Toast.LENGTH_LONG).show();
                }else{
                    resultTxt.setText((String)(msg.obj));
                }
                break;
        }
    }

    @Override
    public void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess logonSuccess, String acc, String pwd, String underWrite, String groupName, String stationPage) {
        mLogonSuccess = logonSuccess;
    }

    @Override
    public void OnSocketMainLogonFail(int errorCode, String errorDescribe) {

    }

    @Override
    public void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess registerSuccess) {
        SharedPreferences share = getSharedPreferences("acccountBaseInfo", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = share.edit();

        editor.putString("account",registerSuccess.getSzAcccount());
        editor.putString("pwd",registerSuccess.getSzPassword());
        editor.putString("phone",registerSuccess.getSzPhone());
        editor.putInt("userId",registerSuccess.getDwUserID());

        editor.commit();

        Message.obtain(handle,1,registerSuccess.getSzPhone()+"号码注册成功").sendToTarget();
    }

    @Override
    public void OnSocketMainRegisterFail(String errorDescribe) {
        Message.obtain(handle,1,errorDescribe).sendToTarget();
    }

    @Override
    public void OnSocketMainUnRegister(int retCode, String retDescribe) {
        Message.obtain(handle,1,retDescribe).sendToTarget();
    }

    @Override
    public void OnSocketMainLoginFinish(){
        if(mLogonSuccess != null && mGameServer != null){
            Bundle b = new Bundle();
            b.putParcelable(UIIntents.UI_INTENT_EXTRA_LOGIN_PLAZA_SUCCESS_DATA,mLogonSuccess);
            b.putParcelableArray(UIIntents.UI_INTENT_EXTRA_GAMEROOM_LIST_DATA,mGameServer);
            b.putString(UIIntents.UI_INTENT_EXTRA_PLAZA_NAME_DATA,mPlazaName);
            UIIntents.get().launchGamePlazaActivity(this,b);
            mLogonSuccess = null;
            mGameServer = null;
            mPlazaName = null;
        }else {
            Message.obtain(handle,1,"登陆失败").sendToTarget();
        }
    }

    @Override
    public void OnSocketMainServerListGameType(tagGameType[] gameType) {

    }

    @Override
    public void OnSocketMainServerListGameKind(tagGameKind[] gameKind) {

    }

    @Override
    public void OnSocketMainServerListGameStation(tagGameStation[] gameStations) {
        mPlazaName = gameStations[0].getSzStationName();
    }

    @Override
    public void OnSocketMainServerListGameServer(tagGameServer[] gameServer) {
        mGameServer = gameServer;
    }

    static class MainHandler extends Handler {
        WeakReference<MainActivity> actRef;

        MainHandler(MainActivity activity) {
            actRef = new WeakReference<MainActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            final MainActivity act = actRef.get();
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
