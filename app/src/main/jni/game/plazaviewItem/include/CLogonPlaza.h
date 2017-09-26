//
// Created by SPREADTRUM\jihao.zhong on 17-6-6.
//

#ifndef CHATFRAME_CLOGONPLAZA_H
#define CHATFRAME_CLOGONPLAZA_H

#include "../../network/include/TcpSocketSink.h"
#include "../../network/include/TCPSocketEnging.h"
#include "../../thread/Thread.h"

//游戏大厅类
class CLogonPlaza : public TcpSocketSink,public Thread
{
private:
    static CLogonPlaza* m_pInstance;
    TCPSocketEnging* m_pSocketEnging;
    char m_pAccount[NAME_LEN];
    char m_pPwd[PASS_LEN];
    char m_phone[PHONE_LEN];
    volatile bool mIsConnect;
    int m_EventType;
    unsigned long m_UserId;
    CMD_GR_LogonByUserID* m_pLogonByID;
    tagClientSerial* m_pClientSerial;
    char mProcessName[32];

private:
    CLogonPlaza();
    CLogonPlaza(const CLogonPlaza&);
    CLogonPlaza& operator=(const CLogonPlaza&);
    ~CLogonPlaza();
    //消息处理
    bool OnEventTCPSocketRead(CMD_Command Command, void* pData, unsigned short wDataSize);
    //登录消息处理
    bool OnSocketMainLogon(CMD_Command Command, void* pData, unsigned short wDataSize);
    //游戏列表消息处理
    bool OnSocketMainServerList(CMD_Command Command, void* pData, unsigned short wDataSize);
    //发送线程实体
    void sendRun();
    //接收线程实体
    void recvRun();
    //断开大厅连接
    void ClosePlaza();

private:
    //大厅登陆回调接口成员
    void (* m_OnLoginSuccessNotify)(CMD_GP_LogonSuccess* logonSuccess, const char* acc, const char* pwd , const char* underWrite, const char* groupName, const char* stationPage);
    void (*m_OnLogonFailNotify)(int errorCode,const char* errorDescribe);
    void (* m_OnRegisterSuccessNotify)(CMD_GP_RegisterSuccess* registerSuccess);
    void (*m_OnRegisterFailNotify)(const char* errorDescribe);
    void (* m_OnGameTypeNotify)(tagGameType* pGameType,int wItemCount);
    void (* m_OnGameKindNotify)(tagGameKind* pGameKind,int wItemCount);
    void (* m_OnGameStationNotify)(tagGameStation* pGameStation,int wItemCount);
    void (* m_OnGameServerNotify)(tagGameServer* pGameServer,int wItemCount);
    void (*m_OnUnRegisterNotify)(int retCode,const char* Describe);
    void (*m_OnLogonFinishNotify)();

public:
    static CLogonPlaza* getInstance();
    //连接大厅服务器
    bool ConnectPlazaServer(const char* serIp,int port);
    //登陆大厅
    void LogonPlaza(const char* acc,const char* pwd,int userId);
    //注册大厅账号
    void RegisterAccount(const char* acc,const char* pwd,const char* phone);
    //注销大厅账号
    void UnRegisterAccount(int uid);
    //接收大厅服务器数据
    void RecvLogonPlazaData(TcpSocketSink* pSocketSink);
    //退出大厅
    void OnExitPlaza();
    //返回进程名
    const char* GetProcessName()const;
    CMD_GR_LogonByUserID* GetLogonId()const;
    tagClientSerial* GetClientSerial()const;
public:
    //大厅登陆回调接口注册
    void OnSetLoginSuccessCallback( void (* loginSuccess)(CMD_GP_LogonSuccess* logonSuccess, const char* acc, const char* pwd , const char* underWrite, const char* groupName, const char* stationPage));
    void OnSetLoginFailCallback( void (* loginFail)(int errorCode,const char* errorDescribe));
    void OnSetRegisterSuccessCallback( void (* registerFunPtr)(CMD_GP_RegisterSuccess* registerSuccess));
    void OnSetRegisterFailCallback( void (* registerFail)(const char* errorDescribe));
    void OnSetGameTypeCallback( void (* pGameType)(tagGameType* gametype,int wItemCount));
    void OnSetGameKindCallback( void (* pGameKind)(tagGameKind* gamekind,int wItemCount));
    void OnSetGameStationCallback( void (* pGameStation)(tagGameStation* gamestation,int wItemCount));
    void OnSetGameServerCallback( void (* pGameServer)(tagGameServer* gameserver,int wItemCount));
    void OnSetUnRegisterCallback( void (* unregister)(int retCode,const char* Describe));
    void OnSetLoginFinishCallback( void (* loginFinish)());

};


#endif //CHATFRAME_CLOGONPLAZA_H
