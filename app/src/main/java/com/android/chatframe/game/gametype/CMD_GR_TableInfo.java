package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-7.
 */

//桌子状态数组
public class CMD_GR_TableInfo {
    short							wTableCount;						  //桌子数目
    tagTableStatus[]				pTableStatus;                         //状态数组

    public CMD_GR_TableInfo(){

    }

    public short getwTableCount() {
        return wTableCount;
    }

    public void setwTableCount(short wTableCount) {
        this.wTableCount = wTableCount;
    }

    public tagTableStatus[] getpTableStatus() {
        return pTableStatus;
    }

    public void setpTableStatus(tagTableStatus[] pTableStatus) {
        this.pTableStatus = pTableStatus;
    }
}
