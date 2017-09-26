package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-7.
 */

//桌子状态结构
public class tagTableStatus {
    byte			bTableLock;							//锁定状态
    byte			bPlayStatus;						//游戏状态

    public tagTableStatus(){

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
