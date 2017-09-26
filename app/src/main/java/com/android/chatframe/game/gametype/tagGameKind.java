package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-8-1.
 */

//游戏种类结构
public class tagGameKind {
    private short						wSortID;                       	//排序号码
    private short						wTypeID;                        //类型号码
    private short						wKindID;                        //名称号码
    private long					    dwMaxVersion;                   //最新版本
    private long						dwOnLineCount;                 	//在线数目
    private String						szKindName;                     //游戏名字
    private String						szProcessName;                  //进程名字

    public tagGameKind(){

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

    public short getwKindID() {
        return wKindID;
    }

    public void setwKindID(short wKindID) {
        this.wKindID = wKindID;
    }

    public long getDwMaxVersion() {
        return dwMaxVersion;
    }

    public void setDwMaxVersion(long dwMaxVersion) {
        this.dwMaxVersion = dwMaxVersion;
    }

    public long getDwOnLineCount() {
        return dwOnLineCount;
    }

    public void setDwOnLineCount(long dwOnLineCount) {
        this.dwOnLineCount = dwOnLineCount;
    }

    public String getSzKindName() {
        return szKindName;
    }

    public void setSzKindName(String szKindName) {
        this.szKindName = szKindName;
    }

    public String getSzProcessName() {
        return szProcessName;
    }

    public void setSzProcessName(String szProcessName) {
        this.szProcessName = szProcessName;
    }
}
