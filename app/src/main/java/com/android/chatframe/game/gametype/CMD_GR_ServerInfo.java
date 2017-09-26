package com.android.chatframe.game.gametype;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-2.
 */

//游戏房间信息
public class CMD_GR_ServerInfo implements Parcelable {
    short					    wKindID;							//类型 I D
    short						wTableCount;						//桌子数目
    short						wChairCount;						//椅子数目
    int 						dwVideoAddr;						//视频地址

    //扩展配置
    short						wGameGenre;							//游戏类型
    char			            cbHideUserInfo;						//隐藏信息

    public CMD_GR_ServerInfo() {

    }

    public short getwKindID() {
        return wKindID;
    }

    public void setwKindID(short wKindID) {
        this.wKindID = wKindID;
    }

    public short getwTableCount() {
        return wTableCount;
    }

    public void setwTableCount(short wTableCount) {
        this.wTableCount = wTableCount;
    }

    public short getwChairCount() {
        return wChairCount;
    }

    public void setwChairCount(short wChairCount) {
        this.wChairCount = wChairCount;
    }

    public int getDwVideoAddr() {
        return dwVideoAddr;
    }

    public void setDwVideoAddr(int dwVideoAddr) {
        this.dwVideoAddr = dwVideoAddr;
    }

    public short getwGameGenre() {
        return wGameGenre;
    }

    public void setwGameGenre(short wGameGenre) {
        this.wGameGenre = wGameGenre;
    }

    public char getCbHideUserInfo() {
        return cbHideUserInfo;
    }

    public void setCbHideUserInfo(char cbHideUserInfo) {
        this.cbHideUserInfo = cbHideUserInfo;
    }

    private CMD_GR_ServerInfo(final Parcel in) {
        wKindID = (short)in.readInt();
        wTableCount = (short)in.readInt();
        wChairCount = (short)in.readInt();
        dwVideoAddr = in.readInt();
        wGameGenre = (short)in.readInt();
        cbHideUserInfo = (char)in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Parcelable.Creator<CMD_GR_ServerInfo> CREATOR
            = new Parcelable.Creator<CMD_GR_ServerInfo>() {
        @Override
        public CMD_GR_ServerInfo createFromParcel(final Parcel in) {
            return new CMD_GR_ServerInfo(in);
        }

        @Override
        public CMD_GR_ServerInfo[] newArray(final int size) {
            return new CMD_GR_ServerInfo[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel out, final int flags) {
        out.writeInt(wKindID);
        out.writeInt(wTableCount);
        out.writeInt(wChairCount);
        out.writeInt(dwVideoAddr);
        out.writeInt(wGameGenre);
        out.writeInt(cbHideUserInfo);
    }
}
