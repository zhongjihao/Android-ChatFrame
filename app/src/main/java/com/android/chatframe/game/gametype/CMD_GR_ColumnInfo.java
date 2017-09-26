package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-2.
 */

//列表描述信息
public class CMD_GR_ColumnInfo {
    short			  wColumnCount;						             //列表数目
    tagColumnItem[]	  pColumnItem;                                   //列表描述


    public CMD_GR_ColumnInfo() {

    }

    public short getwColumnCount() {
        return wColumnCount;
    }

    public void setwColumnCount(short wColumnCount) {
        this.wColumnCount = wColumnCount;
    }

    public tagColumnItem[] getpColumnItem() {
        return pColumnItem;
    }

    public void setpColumnItem(tagColumnItem[] pColumnItem) {
        this.pColumnItem = pColumnItem;
    }
}
