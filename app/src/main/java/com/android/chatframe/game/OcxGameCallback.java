package com.android.chatframe.game;

import com.android.chatframe.game.gametype.CMD_GP_LogonSuccess;
import com.android.chatframe.game.gametype.CMD_GP_RegisterSuccess;
import com.android.chatframe.game.gametype.CMD_GR_ColumnInfo;
import com.android.chatframe.game.gametype.CMD_GR_ServerInfo;
import com.android.chatframe.game.gametype.CMD_GR_UserStatus;
import com.android.chatframe.game.gametype.tagGameKind;
import com.android.chatframe.game.gametype.tagGameServer;
import com.android.chatframe.game.gametype.tagGameStation;
import com.android.chatframe.game.gametype.tagGameType;
import com.android.chatframe.game.gametype.tagUserData;
import com.android.chatframe.game.gametype.CMD_GR_TableStatus;
import com.android.chatframe.game.gametype.CMD_GR_TableInfo;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-6-6.
 */

public interface OcxGameCallback {
    ////////////////游戏大厅登陆响应接口//////////////////////////////////////////
    /**
     *  登陆成功响应
     * @param logonSuccess - 登陆成功数据
     * @param acc - 登陆账号
     * @param pwd - 登陆密码
     * @param underWrite - 个性签名
     * @param groupName - 社团名字
     * @param stationPage - 游戏主站
     */
    public void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess logonSuccess, String acc, String pwd ,String underWrite,String groupName,String stationPage);

    /**
     *  登陆失败响应
     * @param errorCode - 错误码
     * @param errorDescribe - 错误描述
     */
    public void OnSocketMainLogonFail(int errorCode,String errorDescribe);

    /**
     *  注册成功响应
     * @param registerSuccess - 注册成功数据
     */
    public void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess registerSuccess);

    /**
     *  注册失败响应
     * @param errorDescribe - 注册失败描述
     */
    public void OnSocketMainRegisterFail(String errorDescribe);

    /**
     *  注销响应
     * @param retCode - 结果码   0---成功  1---失败
     * @param retDescribe - 结果描述
     */
    public void OnSocketMainUnRegister(int retCode,String retDescribe);

    /**
     *  游戏类型响应
     * @param gameType - 游戏类型数据
     */
    public void OnSocketMainServerListGameType(tagGameType[] gameType);

    /**
     *  游戏种类响应
     * @param gameKind - 游戏种类数据
     */
    public void OnSocketMainServerListGameKind(tagGameKind[] gameKind);

    /**
     *  游戏站点响应
     * @param gameStations - 游戏站点数据
     */
    public void OnSocketMainServerListGameStation(tagGameStation[] gameStations);

    /**
     *  游戏房间列表响应
     * @param gameServer - 游戏房间列表数据
     */
    public void OnSocketMainServerListGameServer(tagGameServer[] gameServer);

    /**
     *  登陆完成响应
     */
    public void OnSocketMainLoginFinish();



    //////////////////游戏房间登陆响应接口//////////////////////////////////////////////
    /**
     *  登陆失败响应
     * @param errorCode - 错误码
     * @param errorDescribe - 错误描述
     */
    public void OnSocketRoomLogonFail(int errorCode,String errorDescribe);

    /**
     *  登陆完成响应
     * @param columnInfo - 列表信息
     * @param serverInfo - 房间信息
     * @param onlineCount - 在线人数
     */
    public void OnSocketRoomLogonFinish(CMD_GR_ColumnInfo columnInfo,CMD_GR_ServerInfo serverInfo,int onlineCount);

    /**
     *  用户进入响应
     * @param userData - 用户信息
     * @param pUnderWrite - 个性签名
     * @param pGroupName - 用户社团
     */
    public void OnSocketRoomSubUserCome(tagUserData userData,String pUnderWrite,String pGroupName);

    /**
     *  用户状态响应
     * @param userStatus - 用户状态信息
     */
    public void OnSocketRoomSubUserStatus(CMD_GR_UserStatus userStatus);

    /**
     *  桌子状态响应
     * @param tableStatus - 桌子状态
     */
    public void OnSocketMainTableStatus(CMD_GR_TableStatus tableStatus);

    /**
     *  桌子信息响应
     * @param tableInfo - 桌子信息
     */
    public void OnSocketMainTableInfo(CMD_GR_TableInfo tableInfo);
}
