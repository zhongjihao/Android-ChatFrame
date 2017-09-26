package com.android.chatframe.game.gametype;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-2.
 */

//用户信息结构
public class tagUserData implements Parcelable {
    //用户属性
    short						wFaceID;                              	//头像索引
    long						dwCustomFaceVer;                        //上传头像
    long						dwUserID;                               //用户 I D
    long						dwGroupID;                              //社团索引
    long						dwGameID;                               //游戏I D
    long						dwUserRight;                            //用户等级
    long						lLoveliness;                            //用户魅力
    long						dwMasterRight;                          //管理权限
    String						szName;                                 //用户名字
    String						szPassword;                             //用户密码
    String						szPhone;                                //用户手机号

    //用户属性
    byte						cbGender;                               //用户性别
    byte						cbMemberOrder;                          //会员等级
    byte						cbMasterOrder;                          //管理等级

    //用户积分
    long						lInsureScore;                          	//消费金币
    long						lGameGold;                              //游戏金币
    long						lScore;                                 //用户分数
    long						lWinCount;                              //胜利盘数
    long						lLostCount;                             //失败盘数
    long						lDrawCount;                             //和局盘数
    long						lFleeCount;                             //断线数目
    long						lExperience;                            //用户经验

    //用户状态
    short		                wMatchRank;							     //排名
    int			                lUsableScore;						     //可用积分
    short						wTableID;                                //桌子号码
    short						wChairID;                                //椅子位置
    byte						cbUserStatus;                            //用户状态
    boolean			            cbIsOut;							     //是否出局

    public tagUserData() {

    }

    public short getwFaceID() {
        return wFaceID;
    }

    public void setwFaceID(short wFaceID) {
        this.wFaceID = wFaceID;
    }

    public long getDwCustomFaceVer() {
        return dwCustomFaceVer;
    }

    public void setDwCustomFaceVer(long dwCustomFaceVer) {
        this.dwCustomFaceVer = dwCustomFaceVer;
    }

    public long getDwUserID() {
        return dwUserID;
    }

    public void setDwUserID(long dwUserID) {
        this.dwUserID = dwUserID;
    }

    public long getDwGroupID() {
        return dwGroupID;
    }

    public void setDwGroupID(long dwGroupID) {
        this.dwGroupID = dwGroupID;
    }

    public long getDwGameID() {
        return dwGameID;
    }

    public void setDwGameID(long dwGameID) {
        this.dwGameID = dwGameID;
    }

    public long getDwUserRight() {
        return dwUserRight;
    }

    public void setDwUserRight(long dwUserRight) {
        this.dwUserRight = dwUserRight;
    }

    public long getlLoveliness() {
        return lLoveliness;
    }

    public void setlLoveliness(long lLoveliness) {
        this.lLoveliness = lLoveliness;
    }

    public long getDwMasterRight() {
        return dwMasterRight;
    }

    public void setDwMasterRight(long dwMasterRight) {
        this.dwMasterRight = dwMasterRight;
    }

    public String getSzName() {
        return szName;
    }

    public void setSzName(String szName) {
        this.szName = szName;
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

    public byte getCbGender() {
        return cbGender;
    }

    public void setCbGender(byte cbGender) {
        this.cbGender = cbGender;
    }

    public byte getCbMemberOrder() {
        return cbMemberOrder;
    }

    public void setCbMemberOrder(byte cbMemberOrder) {
        this.cbMemberOrder = cbMemberOrder;
    }

    public byte getCbMasterOrder() {
        return cbMasterOrder;
    }

    public void setCbMasterOrder(byte cbMasterOrder) {
        this.cbMasterOrder = cbMasterOrder;
    }

    public long getlInsureScore() {
        return lInsureScore;
    }

    public void setlInsureScore(long lInsureScore) {
        this.lInsureScore = lInsureScore;
    }

    public long getlGameGold() {
        return lGameGold;
    }

    public void setlGameGold(long lGameGold) {
        this.lGameGold = lGameGold;
    }

    public long getlScore() {
        return lScore;
    }

    public void setlScore(long lScore) {
        this.lScore = lScore;
    }

    public long getlWinCount() {
        return lWinCount;
    }

    public void setlWinCount(long lWinCount) {
        this.lWinCount = lWinCount;
    }

    public long getlLostCount() {
        return lLostCount;
    }

    public void setlLostCount(long lLostCount) {
        this.lLostCount = lLostCount;
    }

    public long getlDrawCount() {
        return lDrawCount;
    }

    public void setlDrawCount(long lDrawCount) {
        this.lDrawCount = lDrawCount;
    }

    public long getlFleeCount() {
        return lFleeCount;
    }

    public void setlFleeCount(long lFleeCount) {
        this.lFleeCount = lFleeCount;
    }

    public long getlExperience() {
        return lExperience;
    }

    public void setlExperience(long lExperience) {
        this.lExperience = lExperience;
    }

    public short getwMatchRank() {
        return wMatchRank;
    }

    public void setwMatchRank(short wMatchRank) {
        this.wMatchRank = wMatchRank;
    }

    public int getlUsableScore() {
        return lUsableScore;
    }

    public void setlUsableScore(int lUsableScore) {
        this.lUsableScore = lUsableScore;
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

    public boolean getCbIsOut() {
        return cbIsOut;
    }

    public void setCbIsOut(boolean cbIsOut) {
        this.cbIsOut = cbIsOut;
    }


    private tagUserData(final Parcel in) {
        wFaceID = (short)in.readInt();
        dwCustomFaceVer = in.readLong();
        dwUserID = in.readLong();
        dwGroupID = in.readLong();
        dwGameID = in.readLong();
        dwUserRight = in.readLong();
        lLoveliness = in.readLong();
        dwMasterRight = in.readLong();
        szName = in.readString();
        szPassword = in.readString();
        szPhone = in.readString();
        cbGender = in.readByte();
        cbMemberOrder = in.readByte();
        cbMasterOrder = in.readByte();
        lInsureScore = in.readLong();
        lGameGold = in.readLong();
        lScore = in.readLong();
        lWinCount = in.readLong();
        lLostCount = in.readLong();
        lDrawCount = in.readLong();
        lFleeCount = in.readLong();
        lExperience = in.readLong();
        wMatchRank = (short)in.readInt();
        lUsableScore = in.readInt();
        wTableID = (short)in.readInt();
        wChairID = (short)in.readInt();
        cbUserStatus = in.readByte();
        cbIsOut = in.readByte() != 0;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        out.writeInt(wFaceID);
        out.writeLong(dwCustomFaceVer);
        out.writeLong(dwUserID);
        out.writeLong(dwGroupID);
        out.writeLong(dwGameID);
        out.writeLong(dwUserRight);
        out.writeLong(lLoveliness);
        out.writeLong(dwMasterRight);
        out.writeString(szName);
        out.writeString(szPassword);
        out.writeString(szPhone);
        out.writeByte(cbGender);
        out.writeByte(cbMemberOrder);
        out.writeByte(cbMasterOrder);
        out.writeLong(lInsureScore);
        out.writeLong(lGameGold);
        out.writeLong(lScore);
        out.writeLong(lWinCount);
        out.writeLong(lLostCount);
        out.writeLong(lDrawCount);
        out.writeLong(lFleeCount);
        out.writeLong(lExperience);
        out.writeInt(wMatchRank);
        out.writeInt(lUsableScore);
        out.writeInt(wTableID);
        out.writeInt(wChairID);
        out.writeByte(cbUserStatus);
        out.writeByte((byte) (cbIsOut ? 1 : 0));
    }

    public static final Parcelable.Creator<tagUserData> CREATOR
            = new Parcelable.Creator<tagUserData>() {
        @Override
        public tagUserData createFromParcel(final Parcel in) {
            return new tagUserData(in);
        }

        @Override
        public tagUserData[] newArray(final int size) {
            return new tagUserData[size];
        }
    };

    @Override
    public String toString() {
        return "tagUserData{" +
                "wFaceID=" + wFaceID +
                ", dwCustomFaceVer=" + dwCustomFaceVer +
                ", dwUserID=" + dwUserID +
                ", dwGroupID=" + dwGroupID +
                ", dwGameID=" + dwGameID +
                ", dwUserRight=" + dwUserRight +
                ", lLoveliness=" + lLoveliness +
                ", dwMasterRight=" + dwMasterRight +
                ", szName='" + szName + '\'' +
                ", szPassword='" + szPassword + '\'' +
                ", szPhone='" + szPhone + '\'' +
                ", cbGender=" + cbGender +
                ", cbMemberOrder=" + cbMemberOrder +
                ", cbMasterOrder=" + cbMasterOrder +
                ", lInsureScore=" + lInsureScore +
                ", lGameGold=" + lGameGold +
                ", lScore=" + lScore +
                ", lWinCount=" + lWinCount +
                ", lLostCount=" + lLostCount +
                ", lDrawCount=" + lDrawCount +
                ", lFleeCount=" + lFleeCount +
                ", lExperience=" + lExperience +
                ", wMatchRank=" + wMatchRank +
                ", lUsableScore=" + lUsableScore +
                ", wTableID=" + wTableID +
                ", wChairID=" + wChairID +
                ", cbUserStatus=" + cbUserStatus +
                ", cbIsOut=" + cbIsOut +
                '}';
    }
}
