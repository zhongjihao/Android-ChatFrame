package com.android.chatframe.game;

import com.android.chatframe.util.LogUtil;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-6-6.
 */

public class GameNativeInterface {

    static {
        LogUtil.d("GameNativeInterface", "=====GameClient=====loadLibrary==1==" );
        System.loadLibrary("gamecore");
        LogUtil.d("GameNativeInterface", "=====GameClient=====loadLibrary==2==" );
    }
    /**
     * 连接游戏大厅服务器
     * @param ip - 大厅服务器ip地址
     * @param port - 大厅服务器端口
     * @return  true - 连接大厅服务器成功
     *          false - 连接大厅服务器失败
     */
    public native static boolean connectGamePlaza(String ip,int port);

    /**
     * 注册账号
     * @param account - 游戏大厅账号
     * @param pwd - 游戏大厅密码
     * @param phone - 手机号
     */
    public native static void registerGamePlaza(String account,String pwd,String phone);

    /**
     * 登陆游戏大厅
     * @param account - 登陆游戏大厅账号
     * @param pwd - 登陆游戏大厅密码
     * @param userId - 用户ID
     *
     */
    public native static void loginGamePlaza(String account,String pwd,int userId);

    /**
     * 注销游戏大厅账号
     * @param userId - 用户ID
     *
     */
    public native static void unregisterGameAccount(int userId);

    /**
     * 连接游戏房间服务器
     * @param ip - 房间服务器ip地址
     * @param port - 房间服务器端口
     * @param index - 房间索引
     * @return  true - 连接房间服务器成功
     *          false - 连接房间服务器失败
     */
    public native static boolean connectGameRoom(String ip,int port,int index);

    /**
     * 登陆游戏房间
     */
    public native static void loginGameRoom();

    /**
     * 坐下请求
     * @param wTableID - 桌子位置
     * @param wChairID - 椅子位置
     */
    public native static void sitDownReq(short wTableID,short wChairID);

    /**
     * 站立请求
     * @param wTableID - 桌子位置
     * @param wChairID - 椅子位置
     */
    public native static void standUpReq(short wTableID,short wChairID);

    /**
     * 退出房间请求
     * @param wTableID - 桌子位置
     * @param wChairID - 椅子位置
     */
    public native static void exitRoomReq(short wTableID,short wChairID);

    /**
     * 释放房间资源
     */
    public native static void deleteRoom();

    /**
     * 注册回调接口
     * @param ocxCallback - 回调接口
     *
     */
    public native static void registerOcxInterface(OcxGameCallback ocxCallback);


}
