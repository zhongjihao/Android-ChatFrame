//
// Created by SPREADTRUM\jihao.zhong on 17-8-5.
//

#ifndef CHATFRAME_CROOMVIEWITEM_H
#define CHATFRAME_CROOMVIEWITEM_H

#include "../../network/include/TcpSocketSink.h"
#include "../../network/include/TCPSocketEnging.h"
#include "../../usermanager/include/CClientUserItem.h"
#include "../../usermanager/include/CClientUserManager.h"
#include "../../datatype/CTableFrame.h"
#include "../../thread/Thread.h"


//游戏房间类
class CRoomViewItem : public TcpSocketSink,public Thread
{
private:
    TCPSocketEnging* m_pSocketEnging;
    volatile bool mIsConnect;
    int m_EventType;
    unsigned long m_UserId;
    char m_cbBuffer[SOCKET_PACKET];
    int mDataSize;

    //服务状态
    int                         m_ServiceStatus;	                //房间信息
    CMD_GR_ColumnInfo           m_ListColumnInfo;	                //列表信息

    //房间配置
    CMD_GR_ServerInfo           m_ServerInfo;
    int                         m_OnLineCount;                      //在线人数

    //辅助变量
    short						 m_wReqTableID;						//请求桌子
    short						 m_wReqChairID;						//请求位置
    short						 m_wFindTableID;				    //查找位置

    //管理变量
    IClientUserItem* 			 m_pMeUserItem;				        //用户信息
    IClientUserManager*		     m_ClientUserManager;	            //用户管理

    CTableFrame*				 m_TableFrame;                      //桌子框架
    char                         m_processName[32];                 //游戏所在进程名
    tagGameServer*               pGameServer;

private:
    //房间登陆回调接口成员
    void (*m_OnLogonFailNotify)(int errorCode,const char* errorDescribe);
    void (*m_OnLogonFinishNotify)(CMD_GR_ColumnInfo* pColumnInfo,CMD_GR_ServerInfo* pServerInfo,int online);
    void (*m_OnUserComeNotify)(tagUserData* pUserData,const char* pUnderWrite,const char* pGroupName);
    void (*m_OnUserStatusNotify)(CMD_GR_UserStatus* pUserStatus);
    void (*m_OnTableStatusNotify)(CMD_GR_TableStatus* pTableStatus);
    void (*m_OnTableInfoNotify)(CMD_GR_TableInfo* pTableInfo);

private:
    CRoomViewItem(const CRoomViewItem&);
    CRoomViewItem& operator=(const CRoomViewItem&);

public:
    CRoomViewItem();
    ~CRoomViewItem();

private:
    //消息处理
    bool OnEventTCPSocketRead(CMD_Command Command, void* pData, unsigned short wDataSize);
    //登录消息
    bool OnSocketMainLogon(CMD_Command Command, void* pData, unsigned short wDataSize);
    //用户消息
    bool OnSocketMainUser(CMD_Command Command, void* pData, unsigned short wDataSize);
    //用户状态
    bool OnSocketSubStatus(CMD_Command Command, void* pData, unsigned short wDataSize);
    //配置消息
    bool OnSocketMainInfo(CMD_Command Command, void* pData, unsigned short wDataSize);
    //状态消息
    bool OnSocketMainStatus(CMD_Command Command, void* pData, unsigned short wDataSize);
    //系统消息
    bool OnSocketMainSystem(CMD_Command Command, void* pData, unsigned short wDataSize);
    //房间消息
    bool OnSocketMainServerInfo(CMD_Command Command, void* pData, unsigned short wDataSize);
    //用户进入
    bool OnSocketSubUserCome(CMD_Command Command, void* pData, unsigned short wDataSize);
    //游戏消息
    bool OnSocketMainGameFrame(CMD_Command Command, void* pData, unsigned short wDataSize);
    //用户更新
    void OnUserItemUpdate(IClientUserItem* pIUserItem);
    //发送线程实体
    void sendRun();
    //接收线程实体
    void recvRun();

public:
    //连接房间服务器
    bool ConnectGameRoomServer(const char* serIp,int port);
    //登陆房间
    void LogonGameRoom(CMD_GR_LogonByUserID* pLogonByID,tagClientSerial* pClientSerial);
    //发送坐下请求
    void SendSitDownRequest(int wTableID,int wChairID);
    //发送站立请求
    void SendStandUpRequest(int wTableID,int wChairID);
    //接受游戏房间服务器返回的数据
    void RecvLogonRoomViewItemData(TcpSocketSink* pSocketSink);
    //退出房间
    void OnExitRoomServer(int wTableID,int wChairID);
    //释放房间资源
    void OnDeleteRoom();

public:
    //房间登陆回调接口注册
    void OnSetLoginFailCallback( void (* loginFail)(int errorCode,const char* errorDescribe));
    void OnSetLoginFinishCallback( void (* loginFinish)(CMD_GR_ColumnInfo* pColumnInfo,CMD_GR_ServerInfo* pServerInfo,int online));
    void OnSetUserComeCallback( void (* pUserCome)(tagUserData* pUserData,const char* pUnderWrite,const char* pGroupName));
    void OnSetUserStatusCallback( void (* FunPtr)(CMD_GR_UserStatus* pUserStatus));
    void OnSetTableStatusCallback( void (* FunPtr)(CMD_GR_TableStatus* pTableStatus));
    void OnSetTableInfoCallback( void (* FunPtr)(CMD_GR_TableInfo* pTableInfo));
};

#endif //CHATFRAME_CROOMVIEWITEM_H
