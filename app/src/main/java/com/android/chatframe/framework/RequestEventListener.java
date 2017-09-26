package com.android.chatframe.framework;

import android.os.Message;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-5-9.
 */

public interface RequestEventListener {
    public void onConnectComplete(Message msg);
    public void onSentSmsComplete(Message msg);
    public void onDialComplete(Message msg);
    public void onAcceptComplete(Message msg);
    public void onHangupComplete(Message msg);
    public void onRejectComplete(Message msg);
    public void onIncoming(Message msg);
    public void onNewMessageArrive(Message msg);
}
