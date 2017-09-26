package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-7-31.
 */

//游戏类型结构
public class tagGameType {
    private short						wSortID;                        //排序号码
    private short						wTypeID;                        //种类号码
    private String					    szTypeName;          	        //种类名字

    public tagGameType(){

    }

    public short getwSortID() {
        return wSortID;
    }

    public void setwSortID(short wSortID) {
        this.wSortID = wSortID;
    }

    public short getwTypeID() {
        return wTypeID;
    }

    public void setwTypeID(short wTypeID) {
        this.wTypeID = wTypeID;
    }

    public String getSzTypeName() {
        return szTypeName;
    }

    public void setSzTypeName(String szTypeName) {
        this.szTypeName = szTypeName;
    }
}
