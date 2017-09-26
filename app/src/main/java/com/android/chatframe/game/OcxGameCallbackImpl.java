package com.android.chatframe.game;

import com.android.chatframe.game.gametype.CMD_GP_LogonSuccess;
import com.android.chatframe.game.gametype.CMD_GP_RegisterSuccess;
import com.android.chatframe.game.gametype.CMD_GR_ColumnInfo;
import com.android.chatframe.game.gametype.CMD_GR_ServerInfo;
import com.android.chatframe.game.gametype.CMD_GR_TableInfo;
import com.android.chatframe.game.gametype.CMD_GR_TableStatus;
import com.android.chatframe.game.gametype.GameConstant;
import com.android.chatframe.game.gametype.tagGameType;
import com.android.chatframe.game.gametype.tagGameKind;
import com.android.chatframe.game.gametype.tagGameStation;
import com.android.chatframe.game.gametype.tagGameServer;
import com.android.chatframe.game.gametype.tagUserData;
import com.android.chatframe.game.gametype.CMD_GR_UserStatus;
import com.android.chatframe.util.LogUtil;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-7-29.
 */

public class OcxGameCallbackImpl implements OcxGameCallback {

    //游戏大厅回调接口
    public interface OcxPlazaEventListener{
        public void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess logonSuccess, String acc, String pwd , String underWrite, String groupName, String stationPage);
        public void OnSocketMainLogonFail(int errorCode,String errorDescribe);
        public void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess registerSuccess);
        public void OnSocketMainRegisterFail(String errorDescribe);
        public void OnSocketMainUnRegister(int retCode,String retDescribe);
        public void OnSocketMainServerListGameType(tagGameType[] gameType);
        public void OnSocketMainServerListGameKind(tagGameKind[] gameKind);
        public void OnSocketMainServerListGameStation(tagGameStation[] gameStations);
        public void OnSocketMainServerListGameServer(tagGameServer[] gameServer);
        public void OnSocketMainLoginFinish();
    }

    //登陆游戏房间回调接口
    public interface OcxLoginRoomEventListener{
        public void OnSocketMainLogonFail(int errorCode,String errorDescribe);
        public void OnSocketMainLoginFinish(CMD_GR_ColumnInfo columnInfo, CMD_GR_ServerInfo serverInfo, int onlineCount);
        public void OnSocketSubUserCome(tagUserData userData, String pUnderWrite, String pGroupName);
    }

    //游戏房间消息回调接口
    public interface OcxRoomMsgEventListener{
        public void OnSocketSubUserStatus(CMD_GR_UserStatus userStatus);
        public void OnSocketMainTableStatus(CMD_GR_TableStatus tableStatus);
        public void OnSocketMainTableInfo(CMD_GR_TableInfo tableInfo);
    }

    private static OcxPlazaEventListener ocxPlazalisten;
    private static OcxLoginRoomEventListener ocxLoginRoomlisten;
    private static OcxRoomMsgEventListener ocxRoomMsglisten;

    public static void registerOcxPlazaEventListener(OcxPlazaEventListener l){
        ocxPlazalisten = l;
    }

    public static void registerOcxLoginRoomEventListener(OcxLoginRoomEventListener l){
        ocxLoginRoomlisten = l;
    }

    public static void registerOcxRoomMsgEventListener(OcxRoomMsgEventListener l){
        ocxRoomMsglisten = l;
    }

    @Override
    public void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess logonSuccess, String acc, String pwd , String underWrite, String groupName, String stationPage){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainLogonSuccess=========" +
                "wFaceID: " + logonSuccess.getwFaceID() + "  cbGender: "+(logonSuccess.getCbGender() == GameConstant.UserGender.GENDER_BOY ? 'M' : 'F')
                +"  cbMember: "+logonSuccess.getCbMember()+"    dwUserID: "+logonSuccess.getDwUserID()+
                "   dwGameID: "+logonSuccess.getDwGameID()+"   dwExperience: "+logonSuccess.getDwExperience()+
                "   lLoveliness: "+logonSuccess.getlLoveliness()+"   lGoldCoin: "+logonSuccess.getlGoldCoin()+
                "    lScore: "+logonSuccess.getlScore()+"   dwCustomFaceVer: "+logonSuccess.getDwCustomFaceVer()+
                "  acc: "+acc+"  pwd: "+pwd+"  underWrite: "+underWrite+"   groupName: "+groupName+"   stationPage: "+stationPage);

        ocxPlazalisten.OnSocketMainLogonSuccess(logonSuccess,acc,pwd,underWrite,groupName,stationPage);
    }

    @Override
    public void OnSocketMainLogonFail(int errorCode,String errorDescribe){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainLogonFail=========" +
                "errorCode: " + errorCode + "  errorDescribe: "+errorDescribe);
        ocxPlazalisten.OnSocketMainLogonFail(errorCode,errorDescribe);
    }

    @Override
    public void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess registerSuccess){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainRegisterSuccess=========" +
                "wFaceID: " + registerSuccess.getwFaceID() + "  cbGender: "+registerSuccess.getCbGender()
                +"  cbMember: "+registerSuccess.getCbMember()+"    dwUserID: "+registerSuccess.getDwUserID()+
                "   dwGameID: "+registerSuccess.getDwGameID()+"   dwExperience: "+registerSuccess.getDwExperience()+
                "   lLoveliness: "+registerSuccess.getlLoveliness()+"   lGoldCoin: "+registerSuccess.getlGoldCoin()+
                "    lScore: "+registerSuccess.getlScore()+"   account: "+registerSuccess.getSzAcccount()+
                "  pwd: "+registerSuccess.getSzPassword()+"  phone: "+registerSuccess.getSzPhone());

        ocxPlazalisten.OnSocketMainRegisterSuccess(registerSuccess);
    }

    @Override
    public void OnSocketMainRegisterFail(String errorDescribe){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainRegisterFail=========errorDescribe: "+errorDescribe);
        ocxPlazalisten.OnSocketMainRegisterFail(errorDescribe);
    }

    @Override
    public void OnSocketMainUnRegister(int retCode,String retDescribe){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainUnRegister=========" +
                "retCode: " + retCode + "  retDescribe: "+retDescribe);
        ocxPlazalisten.OnSocketMainUnRegister(retCode,retDescribe);
    }

    @Override
    public void OnSocketMainLoginFinish(){
        LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainLoginFinish=========");
        ocxPlazalisten.OnSocketMainLoginFinish();
    }

    @Override
    public void OnSocketMainServerListGameType(tagGameType[] gameType){
        for(int i=0;i< gameType.length;i++){
            LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainServerListGameType=========" +
                    "wSortID: " + gameType[i].getwSortID() + "  wTypeID: "+gameType[i].getwTypeID() +
                    "  szTypeName: "+gameType[i].getSzTypeName());
        }
        ocxPlazalisten.OnSocketMainServerListGameType(gameType);
    }

    @Override
    public void OnSocketMainServerListGameKind(tagGameKind[] gameKind) {
        for (int i = 0; i < gameKind.length; i++) {
            LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainServerListGameKind=========" +
                    "wSortID: " + gameKind[i].getwSortID() + "  wTypeID: " + gameKind[i].getwTypeID() + "   kindId: " + gameKind[i].getwKindID() +
                    "  MaxVersion: " + gameKind[i].getDwMaxVersion()
                    + "   OnLineCount: " + gameKind[i].getDwOnLineCount() +
                    "   KindName : " + gameKind[i].getSzKindName()
                    + "   ProcessName: " + gameKind[i].getSzProcessName());
        }
        ocxPlazalisten.OnSocketMainServerListGameKind(gameKind);
    }

    @Override
    public void OnSocketMainServerListGameStation(tagGameStation[] gameStations) {
        for (int i = 0; i < gameStations.length; i++) {
            LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainServerListGameStation=========" +
                    "wSortID: " + gameStations[i].getwSortID() + "  wKindID: " + gameStations[i].getwKindID() + "   JoinID: " + gameStations[i].getwJoinID() +
                    "  StationID: " + gameStations[i].getwStationID()
                    + "   StationName: " + gameStations[i].getSzStationName());
        }
        ocxPlazalisten.OnSocketMainServerListGameStation(gameStations);
    }

    @Override
    public void OnSocketMainServerListGameServer(tagGameServer[] gameServer) {
        for (int i = 0; i < gameServer.length; i++) {
            LogUtil.d("OcxGameCallbackImpl", "=====GameClient=======OnSocketMainServerListGameServer=========" +
                    "wServerType: " + gameServer[i].getwServerType() + "  SortID: " + gameServer[i].getwSortID() + "   KindID: " + gameServer[i].getwKindID() +
                    "  ServerID: " + gameServer[i].getwServerID() +
                    "   StationID: "+gameServer[i].getwStationID() +
                    "   ServerPort: "+gameServer[i].getwServerPort() +
                    "   ServerAddr :"+gameServer[i].getDwServerAddr() +
                    "   OnLineCount: "+gameServer[i].getDwOnLineCount()
                    + "   ServerName: " + gameServer[i].getSzServerName());
        }
        ocxPlazalisten.OnSocketMainServerListGameServer(gameServer);
    }

    @Override
    public void OnSocketRoomLogonFail(int errorCode, String errorDescribe) {
        LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnRoomMainLogonFail==errorCode: "+errorCode+"  errorDescribe: "+errorDescribe);
        ocxLoginRoomlisten.OnSocketMainLogonFail(errorCode,errorDescribe);
    }

    @Override
    public void OnSocketRoomLogonFinish(CMD_GR_ColumnInfo columnInfo, CMD_GR_ServerInfo serverInfo, int onlineCount) {
        for(int i=0;i<columnInfo.getwColumnCount();i++){
            LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketRoomLogonFinish==" +
                    "cbDataDescribe: "+columnInfo.getpColumnItem()[i].getCbDataDescribe()+
                    "   szColumnName: "+columnInfo.getpColumnItem()[i].getSzColumnName()+
                    "   wColumnWidth: "+columnInfo.getpColumnItem()[i].getwColumnWidth()+
                    "   nFormat: "+columnInfo.getpColumnItem()[i].getnFormat());
        }
        LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketRoomLogonFinish==" +
                "wKindID: "+serverInfo.getwKindID()+"  wTableCount: "+serverInfo.getwTableCount()+
                "   wChairCount: "+serverInfo.getwChairCount()+"    dwVideoAddr: "+serverInfo.getDwVideoAddr()+
                "   wGameGenre: "+serverInfo.getwGameGenre()+"   cbHideUserInfo: "+serverInfo.getCbHideUserInfo()+
                "   onlineCount: "+onlineCount);
        ocxLoginRoomlisten.OnSocketMainLoginFinish(columnInfo,serverInfo,onlineCount);
    }

    @Override
    public void OnSocketRoomSubUserCome(tagUserData userData, String pUnderWrite, String pGroupName) {
        LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketRoomSubUserCome====pUnderWrite: "+pUnderWrite+
                    "    pGroupName: "+pGroupName+
                    "    userData: "+userData.toString());
        ocxLoginRoomlisten.OnSocketSubUserCome(userData,pUnderWrite,pGroupName);
    }

    @Override
    public void OnSocketRoomSubUserStatus(CMD_GR_UserStatus userStatus){
        LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketRoomSubUserStatus==" +
                "dwUserID: "+userStatus.getDwUserID()+"  wTableID: "+userStatus.getwTableID()+
                "   wChairID: "+userStatus.getwChairID()+"    cbUserStatus: "+userStatus.getCbUserStatus());
        ocxRoomMsglisten.OnSocketSubUserStatus(userStatus);
    }

    @Override
    public void OnSocketMainTableStatus(CMD_GR_TableStatus tableStatus) {
        LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketMainTableStatus==" +
                "wTableID: "+tableStatus.getwTableID()+"  bTableLock: "+tableStatus.getbTableLock()+
                "   bPlayStatus: "+tableStatus.getbPlayStatus());
        ocxRoomMsglisten.OnSocketMainTableStatus(tableStatus);
    }

    @Override
    public void OnSocketMainTableInfo(CMD_GR_TableInfo tableInfo) {
        for(int i=0;i<tableInfo.getwTableCount();i++){
            LogUtil.d("OcxGameCallbackImpl", "==GameClient===OnSocketMainTableInfo==" +
                    "bTableLock: "+tableInfo.getpTableStatus()[i].getbTableLock()+
                    "   bPlayStatus: "+tableInfo.getpTableStatus()[i].getbPlayStatus());
        }
        ocxRoomMsglisten.OnSocketMainTableInfo(tableInfo);
    }
}
