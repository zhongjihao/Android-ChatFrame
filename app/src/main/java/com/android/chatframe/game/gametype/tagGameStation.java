package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-8-1.
 */

//游戏站点结构
public class tagGameStation {
    private short						wSortID;                       //排序号码
    private short						wKindID;                       //名称号码
    private short 						wJoinID;                       //挂接号码
    private short						wStationID;                    //站点号码
    private String						szStationName;                 //站点名称

    public tagGameStation(){

    }

    public short getwSortID() {
        return wSortID;
    }

    public void setwSortID(short wSortID) {
        this.wSortID = wSortID;
    }

    public short getwKindID() {
        return wKindID;
    }

    public void setwKindID(short wKindID) {
        this.wKindID = wKindID;
    }

    public short getwJoinID() {
        return wJoinID;
    }

    public void setwJoinID(short wJoinID) {
        this.wJoinID = wJoinID;
    }

    public short getwStationID() {
        return wStationID;
    }

    public void setwStationID(short wStationID) {
        this.wStationID = wStationID;
    }

    public String getSzStationName() {
        return szStationName;
    }

    public void setSzStationName(String szStationName) {
        this.szStationName = szStationName;
    }
}
