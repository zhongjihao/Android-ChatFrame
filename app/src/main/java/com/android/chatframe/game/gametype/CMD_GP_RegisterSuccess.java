package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-7-31.
 */

//注册成功响应数据
public class CMD_GP_RegisterSuccess {
    private short						    wFaceID;						//头像索引
    private byte                            cbGender;						//用户性别
    private byte					        cbMember;						//会员等级
    private int								dwUserID;						//用户 I D
    private int								dwGameID;						//游戏 I D
    private int								dwExperience;					//用户经验

    private int 							lLoveliness;					//用户魅力
    private int 							lGoldCoin;						//用户金币
    private int 							lScore;						    //用户积分

    private String                          szAcccount;                     //账号
    private String                          szPassword;                     //密码
    private String                          szPhone;                        //手机号

    public CMD_GP_RegisterSuccess(){

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

    public String getSzAcccount() {
        return szAcccount;
    }

    public void setSzAcccount(String szAcccount) {
        this.szAcccount = szAcccount;
    }

    public String getSzPassword() {
        return szPassword;
    }

    public void setSzPassword(String szPassword) {
        this.szPassword = szPassword;
    }

    public String getSzPhone() {
        return szPhone;
    }

    public void setSzPhone(String szPhone) {
        this.szPhone = szPhone;
    }


}
