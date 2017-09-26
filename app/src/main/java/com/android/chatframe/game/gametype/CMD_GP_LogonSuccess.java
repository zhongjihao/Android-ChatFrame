package com.android.chatframe.game.gametype;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-7-29.
 */

//登陆成功
public class CMD_GP_LogonSuccess implements Parcelable{
    private short						    wFaceID;						//头像索引
    private byte                            cbGender;						//用户性别
    private byte					        cbMember;						//会员等级
    private int								dwUserID;						//用户 I D
    private int								dwGameID;						//游戏 I D
    private int								dwExperience;					//用户经验

    private int 							lLoveliness;					//用户魅力
    private int 							lGoldCoin;						//用户金币
    private int 							lScore;						    //用户积分

    //扩展信息
    private int								dwCustomFaceVer;				//头像版本

    public CMD_GP_LogonSuccess(){

    }

    public short getwFaceID() {
        return wFaceID;
    }

    public void setwFaceID(short wFaceID) {
        this.wFaceID = wFaceID;
    }

    public byte getCbGender() {
        return cbGender;
    }

    public void setCbGender(byte cbGender) {
        this.cbGender = cbGender;
    }

    public byte getCbMember() {
        return cbMember;
    }

    public void setCbMember(byte cbMember) {
        this.cbMember = cbMember;
    }

    public int getDwUserID() {
        return dwUserID;
    }

    public void setDwUserID(int dwUserID) {
        this.dwUserID = dwUserID;
    }

    public int getDwGameID() {
        return dwGameID;
    }

    public void setDwGameID(int dwGameID) {
        this.dwGameID = dwGameID;
    }

    public int getDwExperience() {
        return dwExperience;
    }

    public void setDwExperience(int dwExperience) {
        this.dwExperience = dwExperience;
    }

    public int getlLoveliness() {
        return lLoveliness;
    }

    public void setlLoveliness(int lLoveliness) {
        this.lLoveliness = lLoveliness;
    }

    public int getlGoldCoin() {
        return lGoldCoin;
    }

    public void setlGoldCoin(int lGoldCoin) {
        this.lGoldCoin = lGoldCoin;
    }

    public int getlScore() {
        return lScore;
    }

    public void setlScore(int lScore) {
        this.lScore = lScore;
    }

    public int getDwCustomFaceVer() {
        return dwCustomFaceVer;
    }

    public void setDwCustomFaceVer(int dwCustomFaceVer) {
        this.dwCustomFaceVer = dwCustomFaceVer;
    }

    private CMD_GP_LogonSuccess(final Parcel in) {
        wFaceID = (short)in.readInt();
        cbGender = in.readByte();
        cbMember = in.readByte();
        dwUserID = in.readInt();
        dwGameID = in.readInt();
        dwExperience = in.readInt();
        lLoveliness = in.readInt();
        lGoldCoin = in.readInt();
        lScore = in.readInt();
        dwCustomFaceVer = in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Parcelable.Creator<CMD_GP_LogonSuccess> CREATOR
            = new Parcelable.Creator<CMD_GP_LogonSuccess>() {
        @Override
        public CMD_GP_LogonSuccess createFromParcel(final Parcel in) {
            return new CMD_GP_LogonSuccess(in);
        }

        @Override
        public CMD_GP_LogonSuccess[] newArray(final int size) {
            return new CMD_GP_LogonSuccess[size];
        }
    };

    @Override
    public void writeToParcel(final Parcel out, final int flags) {
        out.writeInt(wFaceID);
        out.writeByte(cbGender);
        out.writeByte(cbMember);
        out.writeInt(dwUserID);
        out.writeInt(dwGameID);
        out.writeInt(dwExperience);
        out.writeInt(lLoveliness);
        out.writeInt(lGoldCoin);
        out.writeInt(lScore);
        out.writeInt(dwCustomFaceVer);
    }
}
