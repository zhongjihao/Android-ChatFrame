package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-7.
 */

//桌子状态信息
public class CMD_GR_TableStatus {
    short				            wTableID;							//桌子号码
    byte					        bTableLock;							//锁定状态
    byte					        bPlayStatus;						//游戏状态

    public CMD_GR_TableStatus(){

    }

    public short getwTableID() {
        return wTableID;
    }

    public void setwTableID(short wTableID) {
        this.wTableID = wTableID;
    }

    public byte getbTableLock() {
        return bTableLock;
    }

    public void setbTableLock(byte bTableLock) {
        this.bTableLock = bTableLock;
    }

    public byte getbPlayStatus() {
        return bPlayStatus;
    }

    public void setbPlayStatus(byte bPlayStatus) {
        this.bPlayStatus = bPlayStatus;
    }
}
