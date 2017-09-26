package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-7.
 */

//用户状态
public class CMD_GR_UserStatus {
    long						dwUserID;							//用户 ID
    short						wTableID;							//桌子位置
    short					    wChairID;							//椅子位置
    byte				        cbUserStatus;						//用户状态

    public CMD_GR_UserStatus(){

    }

    public long getDwUserID() {
        return dwUserID;
    }

    public void setDwUserID(long dwUserID) {
        this.dwUserID = dwUserID;
    }

    public short getwTableID() {
        return wTableID;
    }

    public void setwTableID(short wTableID) {
        this.wTableID = wTableID;
    }

    public short getwChairID() {
        return wChairID;
    }

    public void setwChairID(short wChairID) {
        this.wChairID = wChairID;
    }

    public byte getCbUserStatus() {
        return cbUserStatus;
    }

    public void setCbUserStatus(byte cbUserStatus) {
        this.cbUserStatus = cbUserStatus;
    }
}
