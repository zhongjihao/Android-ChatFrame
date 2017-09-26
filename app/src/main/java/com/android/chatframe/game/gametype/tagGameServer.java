package com.android.chatframe.game.gametype;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-8-1.
 */

//游戏房间列表结构
public class tagGameServer implements Parcelable {
    private short                       wServerType;                  //房间类型
    private short						wSortID;                      //排序号码
    private short						wKindID;                      //名称号码
    private short						wServerID;                    //房间号码
    private short						wStationID;                   //站点号码
    private short						wServerPort;                  //房间端口
    private String						dwServerAddr;                 //房间地址
    private long						dwOnLineCount;                //在线人数
    private String						szServerName;                 //房间名称

    public tagGameServer(){

    }

    public short getwServerType() {
        return wServerType;
    }

    public void setwServerType(short wServerType) {
        this.wServerType = wServerType;
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

    public short getwServerID() {
        return wServerID;
    }

    public void setwServerID(short wServerID) {
        this.wServerID = wServerID;
    }

    public short getwStationID() {
        return wStationID;
    }

    public void setwStationID(short wStationID) {
        this.wStationID = wStationID;
    }

    public short getwServerPort() {
        return wServerPort;
    }

    public void setwServerPort(short wServerPort) {
        this.wServerPort = wServerPort;
    }

    public String getDwServerAddr() {
        return dwServerAddr;
    }

    public void setDwServerAddr(String dwServerAddr) {
        this.dwServerAddr = dwServerAddr;
    }

    public long getDwOnLineCount() {
        return dwOnLineCount;
    }

    public void setDwOnLineCount(long dwOnLineCount) {
        this.dwOnLineCount = dwOnLineCount;
    }

    public String getSzServerName() {
        return szServerName;
    }

    public void setSzServerName(String szServerName) {
        this.szServerName = szServerName;
    }

    private tagGameServer(final Parcel in) {
        wServerType = (short)in.readInt();
        wSortID = (short)in.readInt();
        wKindID = (short)in.readInt();
        wServerID = (short)in.readInt();
        wStationID = (short)in.readInt();
        wServerPort = (short)in.readInt();
        dwServerAddr = in.readString();
        dwOnLineCount = in.readLong();
        szServerName = in.readString();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Parcelable.Creator<tagGameServer> CREATOR
            = new Parcelable.Creator<tagGameServer>() {
        @Override
        public tagGameServer createFromParcel(final Parcel in) {
            return new tagGameServer(in);
        }

        @Override
        public tagGameServer[] newArray(final int size) {
            return new tagGameServer[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel out, final int flags) {
        out.writeInt(wServerType);
        out.writeInt(wSortID);
        out.writeInt(wKindID);
        out.writeInt(wServerID);
        out.writeInt(wStationID);
        out.writeInt(wServerPort);
        out.writeString(dwServerAddr);
        out.writeLong(dwOnLineCount);
        out.writeString(szServerName);
    }
}
