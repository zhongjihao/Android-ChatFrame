//
// Created by SPREADTRUM\jihao.zhong on 17-6-6.
//

#define LOG_TAG "CLogonPlaza"

#include <assert.h>
#include "../../../log.h"
#include "../include/CLogonPlaza.h"
#include "../../network/include/CRecvPacketHelper.h"
#include "../../network/include/CSendPacketHelper.h"

CLogonPlaza* CLogonPlaza::m_pInstance = new CLogonPlaza;

CLogonPlaza::CLogonPlaza() : mIsConnect(false),m_EventType(0),m_UserId(INVALID_USER_ID)
{
    m_pSocketEnging = new TCPSocketEnging;
    m_pLogonByID = new CMD_GR_LogonByUserID;
    m_pClientSerial = new tagClientSerial;
    memset(m_pAccount,0, sizeof(m_pAccount));
    memset(m_pPwd,0, sizeof(m_pPwd));
    memset(m_phone,0, sizeof(m_phone));
    memset(mProcessName,0, sizeof(mProcessName));

    LOGD("%s:====GameClient========",__FUNCTION__);
    //启动接收线程
    startRecvThread();
    //启动发送线程
    startSendThread();
}

CLogonPlaza::~CLogonPlaza()
{
    LOGD("%s:====GameClient========",__FUNCTION__);
    if(m_pSocketEnging)
    {
        delete m_pSocketEnging;
        m_pSocketEnging =  NULL;
    }
    if(m_pLogonByID)
    {
        delete m_pLogonByID;
        m_pLogonByID = NULL;
    }

    if(m_pClientSerial)
    {
        delete m_pClientSerial;
        m_pClientSerial = NULL;
    }
}

CLogonPlaza* CLogonPlaza::getInstance()
{
    return m_pInstance;
}

bool CLogonPlaza::ConnectPlazaServer(const char* serIp,int port)
{
    mIsConnect = m_pSocketEnging->ConnectServer(serIp,port);
    LOGD("%s: ======GameClient=======m_isRecvRuning: %d, mIsConnect: %d" ,__FUNCTION__,m_isRecvRuning,mIsConnect);
    return mIsConnect;
}

void CLogonPlaza::LogonPlaza(const char* acc,const char* pwd,int userId)
{
    assert(strlen(acc) < NAME_LEN);
    assert(strlen(pwd) < PASS_LEN);
    memcpy(m_pAccount,acc,strlen(acc));
    memcpy(m_pPwd,pwd,strlen(pwd));
    m_EventType = LOGON_MSG;
    m_UserId = userId;
}

void CLogonPlaza::RegisterAccount(const char* acc,const char* pwd,const char* phone)
{
    assert(strlen(acc) < NAME_LEN);
    assert(strlen(pwd) < PASS_LEN);
    assert(strlen(phone) < PHONE_LEN);
    LOGD("%s:====GameClient====1====账号: %s,  密码: %s, 手机号: %s",__FUNCTION__,acc,pwd,phone);
    memcpy(m_pAccount,acc,strlen(acc));
    memcpy(m_pPwd,pwd,strlen(pwd));
    memcpy(m_phone,phone,strlen(phone));
    LOGD("%s:====GameClient=====2===账号: %s,  密码: %s, 手机号: %s",__FUNCTION__,m_pAccount,m_pPwd,m_phone);
    m_EventType = REGISTER_MSG;
}

void CLogonPlaza::UnRegisterAccount(int userId)
{
    m_EventType = UNREGISTER_MSG;
    m_UserId = userId;
}

void CLogonPlaza::OnExitPlaza()
{
    m_EventType = EXIT_PLAZA_MSG;
}

void CLogonPlaza::OnSetLoginSuccessCallback( void (* loginSuccess)(CMD_GP_LogonSuccess* logonSuccess, const char* acc, const char* pwd , const char* underWrite, const char* groupName, const char* stationPage))
{
    m_OnLoginSuccessNotify = loginSuccess;
}

void CLogonPlaza::OnSetLoginFailCallback( void (* loginFail)(int errorCode,const char* errorDescribe))
{
    m_OnLogonFailNotify = loginFail;
}

void CLogonPlaza::OnSetRegisterSuccessCallback( void (* registerSuccess)(CMD_GP_RegisterSuccess* registerSuccess))
{
    m_OnRegisterSuccessNotify = registerSuccess;
}

void CLogonPlaza::OnSetRegisterFailCallback( void (* registerFail)(const char* errorDescribe))
{
    m_OnRegisterFailNotify = registerFail;
}

void CLogonPlaza::OnSetGameTypeCallback( void (* pGameType)(tagGameType* gametype,int wItemCount))
{
    m_OnGameTypeNotify = pGameType;
}

void CLogonPlaza::OnSetGameKindCallback( void (* pGameKind)(tagGameKind* gamekind,int wItemCount))
{
    m_OnGameKindNotify = pGameKind;
}

void CLogonPlaza::OnSetGameStationCallback( void (* pGameStation)(tagGameStation* gamestation,int wItemCount))
{
    m_OnGameStationNotify = pGameStation;
}

void CLogonPlaza::OnSetGameServerCallback( void (* pGameServer)(tagGameServer* gameserver,int wItemCount))
{
    m_OnGameServerNotify = pGameServer;
}

void CLogonPlaza::OnSetUnRegisterCallback( void (* unregister)(int retCode,const char* Describe))
{
    m_OnUnRegisterNotify = unregister;
}

void CLogonPlaza::OnSetLoginFinishCallback( void (* loginFinish)())
{
    m_OnLogonFinishNotify = loginFinish;
}

const char* CLogonPlaza::GetProcessName()const
{
    return mProcessName;
}

CMD_GR_LogonByUserID* CLogonPlaza::GetLogonId()const
{
    return m_pLogonByID;
}

tagClientSerial* CLogonPlaza::GetClientSerial()const
{
    return m_pClientSerial;
}

void CLogonPlaza::sendRun()
{
    while(m_isSendRuning)
    {
        switch (m_EventType)
        {
            case REGISTER_MSG:
            {
                //构造注册消息包
                unsigned char data[SOCKET_BUFFER];
                CSendPacketHelper Packet(data, SOCKET_BUFFER);
                //添加用户账号
                Packet.AddPacket(m_pAccount,strlen(m_pAccount),DTP_USER_ACCOUNTS);
                //添加用户密码
                Packet.AddPacket(m_pPwd,strlen(m_pPwd),DTP_USER_PASS);
                //添加手机号码
                Packet.AddPacket(m_phone,strlen(m_phone),DTP_USER_PHONE);
                //添加结束标志
                Packet.AddPacket(NULL,0,0);

                LOGD("%s:====GameClient========附加数据大小: %d",__FUNCTION__,Packet.GetDataSize());
                //发送注册消息包
                m_pSocketEnging->SendData(MDM_GP_LOGON,SUB_GP_REGISTER_ACCOUNTS,data,Packet.GetDataSize());
                m_EventType = 0;
                break;
            }
            case LOGON_MSG:
            {
                //构造登陆包
                LoginAccount logAcc;
                memset(m_pLogonByID,0,sizeof(*m_pLogonByID));
                memset(&logAcc,0, sizeof(LoginAccount));
                m_pLogonByID->dwPlazaVersion = logAcc.dwPlazaVersion = PLAZA_VERSION;
                m_pLogonByID->dwProcessVersion = PROCESS_VERSION;//进程版本
                m_pLogonByID->dwUserID = logAcc.dwUserId = m_UserId;
                memcpy(m_pLogonByID->szPassWord,m_pPwd,strlen(m_pPwd));
                memcpy(logAcc.szAccounts,m_pAccount,strlen(m_pAccount));
                memcpy(logAcc.szPassWord,m_pPwd,strlen(m_pPwd));

                memset(m_pClientSerial,0,sizeof(tagClientSerial));
                m_pClientSerial->dwSystemVer = 65541;
                m_pClientSerial->dwComputerID[0] = 1734166096;
                m_pClientSerial->dwComputerID[1] = 47480;
                m_pClientSerial->dwComputerID[2] = 0;
                int dataSize = sizeof(LoginAccount) + sizeof(tagClientSerial);
                unsigned char data[dataSize];
                memcpy(data,&logAcc, sizeof(LoginAccount));
                memcpy(data+sizeof(LoginAccount),m_pClientSerial, sizeof(tagClientSerial));

                //发送登陆包
                LOGD("%s: ==GameClient===登陆大厅数据大小==dataSize: %d, 账号长度: %zu, 密码长度: %zu, dwUserId: %lu",__FUNCTION__,
                     dataSize,strlen(logAcc.szAccounts),strlen(logAcc.szPassWord),m_UserId);
                LoginAccount* acc = (LoginAccount*)data;
                LOGD("%s: ===GameClient=====dwPlazaVersion: %d,  szAccounts: %s,  szPassWord: %s , dwUserId: %lu",__FUNCTION__,acc->dwPlazaVersion,acc->szAccounts,acc->szPassWord,acc->dwUserId);

                tagClientSerial* tag = (tagClientSerial*)(acc+1);

                LOGD("%s: ===GameClient=====dwSystemVer: %lu,  dwComputerID[0] :%lu,  dwComputerID[1]: %lu,  dwComputerID[2]: %lu\n ",__FUNCTION__,tag->dwSystemVer,tag->dwComputerID[0],tag->dwComputerID[1],tag->dwComputerID[2]);

                m_pSocketEnging->SendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,data,dataSize);
                m_EventType = 0;
                break;
            }
            case UNREGISTER_MSG:
            {
                //注销账号
                int unregisterId = m_UserId;
                m_pSocketEnging->SendData(MDM_GP_LOGON,SUB_GP_UNREGISTER_ACCOUNTS,&unregisterId,sizeof(unregisterId));
                m_EventType = 0;
                break;
            }
            case EXIT_PLAZA_MSG:
            {
                //下线通知
                int unregisterId = m_UserId;
                m_pSocketEnging->SendData(MDM_GP_LOGON,SUB_GP_EXIT_PLAZA,&unregisterId,sizeof(unregisterId));
                m_EventType = 0;
                m_isRecvRuning = false;
                m_isSendRuning = false;
                ClosePlaza();
                break;
            }
        }
    }
    LOGD("%s: ======GameClient====大厅发送线程退出====" ,__FUNCTION__);
}

void CLogonPlaza::recvRun()
{
    LOGD("%s: ======GameClient===大厅接收线程=====m_isRecvRuning: %d, mIsConnect: %d" ,__FUNCTION__,m_isRecvRuning,mIsConnect);
    while(m_isRecvRuning)
    {
        if(mIsConnect)
        {
            //接收大厅服务器返回数据
            RecvLogonPlazaData(this);
        }
    }
    LOGD("%s: ======GameClient====大厅接收线程退出====" ,__FUNCTION__);
}

void CLogonPlaza::RecvLogonPlazaData(TcpSocketSink* pSocketSink)
{
    m_pSocketEnging ->OnSocketNotifyRead(pSocketSink);
}

bool CLogonPlaza::OnEventTCPSocketRead(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    switch (Command.wMainCmdID)
    {
        case MDM_GP_LOGON:				//登录消息
        {
            return OnSocketMainLogon(Command,pData,wDataSize);
        }
        case MDM_GP_SERVER_LIST:		   //列表消息
        {
            return OnSocketMainServerList(Command,pData,wDataSize);
        }
    }

    return true;
}

//登录消息
bool CLogonPlaza::OnSocketMainLogon(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GP_LOGON);
    switch (Command.wSubCmdID)
    {
        case SUB_GP_LOGON_SUCCESS:		//登录成功
        {
            //效验参数
            assert(wDataSize >=sizeof(CMD_GP_LogonSuccess));
            if (wDataSize <  sizeof(CMD_GP_LogonSuccess))
            {
                return false;
            }

            //保存信息
            CMD_GP_LogonSuccess* logininfo = (CMD_GP_LogonSuccess*)pData;
            LOGD("%s: ==GameClient=====wFaceID: %d, cbGender: %c, cbMember: %c,"
                         "dwUserID: %d, dwGameID: %d, dwExperience: %d,"
                         "lLoveliness: %d,lGoldCoin: %d,lScore: %d , dwCustomFaceVer: %d",__FUNCTION__,
                 logininfo->wFaceID,(logininfo->cbGender == GENDER_BOY ? 'M' : 'F'),
                 logininfo->cbMember,logininfo->dwUserID,logininfo->dwGameID,
                 logininfo->dwExperience,logininfo->lLoveliness,logininfo->lGoldCoin,
                 logininfo->lScore,logininfo->dwCustomFaceVer);

            //扩展信息
            void* pDataBuffer = NULL;
            tagDataDescribe DataDescribe;
            void* pLogonSuccess = NULL;
            pLogonSuccess = (void*)(logininfo+1);
            char szAccounts[NAME_LEN];
            char szPassWord[PASS_LEN];
            char szUnderWrite[UNDER_WRITE_LEN];
            char szGroupName[GROUP_LEN];
            char szStationPage[STATION_PAGE_LEN];
            memset(szAccounts,0, sizeof(szAccounts));
            memset(szPassWord,0, sizeof(szPassWord));
            memset(szUnderWrite,0, sizeof(szUnderWrite));
            memset(szGroupName,0, sizeof(szGroupName));
            memset(szStationPage,0, sizeof(szStationPage));

            CRecvPacketHelper RecvPacket(pLogonSuccess,wDataSize-sizeof(CMD_GP_LogonSuccess));
            while(true)
            {
                pDataBuffer = RecvPacket.GetData(DataDescribe);
                if(DataDescribe.wDataDescribe == 0) break;
                switch (DataDescribe.wDataDescribe)
                {
                    case DTP_USER_ACCOUNTS:		//用户帐户
                    {
                        assert(DataDescribe.wDataSize <= NAME_LEN);
                        assert(pDataBuffer != NULL);
                        assert(DataDescribe.wDataSize > 0);
                        memcpy(szAccounts,pDataBuffer,DataDescribe.wDataSize);
                        szAccounts[DataDescribe.wDataSize] = 0;
                        LOGD("%s: ==GameClient=====账号: %s",__FUNCTION__,szAccounts);
                        break;
                    }
                    case DTP_USER_PASS:			//用户密码
                    {
                        assert(pDataBuffer != NULL);
                        assert(DataDescribe.wDataSize > 0);
                        assert(DataDescribe.wDataSize <= PASS_LEN);
                        memcpy(szPassWord,pDataBuffer,DataDescribe.wDataSize);
                        szPassWord[DataDescribe.wDataSize] = 0;
                        LOGD("%s: ==GameClient=====密码: %s",__FUNCTION__,szPassWord);
                        break;
                    }
                    case DTP_UNDER_WRITE:		//个性签名
                    {
                        assert(pDataBuffer != NULL);
                        assert(DataDescribe.wDataSize > 0);
                        assert(DataDescribe.wDataSize <= UNDER_WRITE_LEN);
                        memcpy(szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
                        szUnderWrite[DataDescribe.wDataSize] = 0;
                        LOGD("%s: ==GameClient=====个性签名: %s",__FUNCTION__,szUnderWrite);
                        break;
                    }
                    case DTP_USER_GROUP_NAME:	//社团名字
                    {
                        assert(pDataBuffer != NULL);
                        assert(DataDescribe.wDataSize > 0);
                        assert(DataDescribe.wDataSize<= GROUP_LEN);
                        memcpy(szGroupName,pDataBuffer,DataDescribe.wDataSize);
                        szGroupName[DataDescribe.wDataSize] = 0;
                        LOGD("%s: ==GameClient=====社团名字: %s",__FUNCTION__,szGroupName);
                        break;
                    }
                    case DTP_STATION_PAGE:		//游戏主站
                    {
                        assert(pDataBuffer != NULL);
                        assert(DataDescribe.wDataSize > 0);
                        assert(DataDescribe.wDataSize<= STATION_PAGE_LEN);
                        memcpy(szStationPage,pDataBuffer,DataDescribe.wDataSize);
                        szStationPage[DataDescribe.wDataSize] = 0;
                        LOGD("%s: ==GameClient=====游戏主站: %s",__FUNCTION__,szStationPage);
                        break;
                    }
                    default:
                        return false;
                }
            }
            //设置提示
            LOGD("%s: ======GameClient====正在读取服务器列表信息...=======",__FUNCTION__);
            (*m_OnLoginSuccessNotify)(logininfo,szAccounts,szPassWord,szUnderWrite,szGroupName,szStationPage);
            return true;
        }
        case SUB_GP_LOGON_ERROR:		//登录失败
        {
            //效验参数
            CMD_GP_LogonError* pLogonError = (CMD_GP_LogonError*)pData;
            assert(wDataSize >= sizeof(CMD_GP_LogonError) - sizeof(pLogonError->szErrorDescribe));

            //显示消息
            int lErrorCode = pLogonError->lErrorCode;
            const char* szErrorDescribe = pLogonError->szErrorDescribe;
            LOGD("%s: ======GameClient====错误码: %d,  错误描述: %s",__FUNCTION__,lErrorCode,szErrorDescribe);
            (*m_OnLogonFailNotify)(lErrorCode,szErrorDescribe);
            return true;
        }
        case SUB_GP_LOGON_FINISH:		//登录完成
        {
            LOGD("%s: =======GameClient=====登陆游戏大厅完成=============",__FUNCTION__);
            //关闭连接
            ClosePlaza();
            (*m_OnLogonFinishNotify)();
            return true;
        }
        case SUB_GP_REGISTER_SUCCESS:    //注册成功
        {
            //效验参数
            assert(wDataSize >=sizeof(CMD_GP_RegisterSuccess));
            if (wDataSize <  sizeof(CMD_GP_RegisterSuccess))
            {
                return false;
            }

            //保存信息
            CMD_GP_RegisterSuccess* reginfo = (CMD_GP_RegisterSuccess*)pData;
            LOGD("%s: ==GameClient=====wFaceID: %d, cbGender: %c, cbMember: %c,"
                         "dwUserID: %d, dwGameID: %d, dwExperience: %d, "
                         "lLoveliness: %d,lGoldCoin: %d,lScore: %d, szAcccount: %s,  szPassword: %s, szPhone: %s",__FUNCTION__,
                 reginfo->wFaceID,reginfo->cbGender == GENDER_BOY ? 'M' : 'F', reginfo->cbMember,reginfo->dwUserID,reginfo->dwGameID,
                 reginfo->dwExperience,reginfo->lLoveliness,reginfo->lGoldCoin,
                 reginfo->lScore,reginfo->szAcccount,reginfo->szPassword,reginfo->szPhone);
            m_UserId = reginfo->dwUserID;
            (*m_OnRegisterSuccessNotify)(reginfo);
            //关闭连接
            ClosePlaza();
            return true;
        }
        case SUB_GP_REGISTER_FAIL:    //注册失败
        {
            char* msg = (char*)pData;
            LOGD("%s: ====GameClient========msg: %s",__FUNCTION__,msg);
            (*m_OnRegisterFailNotify)(msg);
            //关闭连接
            ClosePlaza();
            return  true;
        }
        case SUB_GP_UNREGISTER_RESPONSE:   //注销响应
        {
            //效验参数
            CMD_GP_UnRegisterResponse* pUnregisterRet = (CMD_GP_UnRegisterResponse*)pData;
            assert(wDataSize >= sizeof(CMD_GP_UnRegisterResponse) - sizeof(pUnregisterRet->szDescribe));

            //关闭连接
            ClosePlaza();
            //显示消息
            int lretCode = pUnregisterRet->retCode;
            const char* szDescribe = pUnregisterRet->szDescribe;
            LOGD("%s: ======GameClient====注销响应码: %d,  注销响应描述: %s",__FUNCTION__,lretCode,szDescribe);
            (*m_OnUnRegisterNotify)(lretCode,szDescribe);
            return true;
        }
    }
    return true;
}

//游戏列表消息
bool CLogonPlaza::OnSocketMainServerList(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GP_SERVER_LIST);
    switch(Command.wSubCmdID)
    {
        case SUB_GP_LIST_TYPE:			//类型信息
        {
            //效验参数
            assert(wDataSize % sizeof(tagGameType) == 0);

            //处理消息
            tagGameType* pGameType = (tagGameType*)pData;
            short wItemCount = (short)(wDataSize/ sizeof(tagGameType));
            for(int i=0;i<wItemCount;i++)
            {
                LOGD("%s: =======GameClient=====类型信息=============wSortID: %d,wTypeID: %d, szTypeName: %s",__FUNCTION__,
                     (pGameType+i)->wSortID,(pGameType+i)->wTypeID,(pGameType+i)->szTypeName);
            }
            (*m_OnGameTypeNotify)(pGameType,wItemCount);
            return true;
        }
        case SUB_GP_LIST_KIND:			//种类消息
        {
            //效验参数
            assert(wDataSize % sizeof(tagGameKind) == 0);

            //处理消息
            tagGameKind* pGameKind = (tagGameKind*)pData;
            short wItemCount = (short)(wDataSize/ sizeof(tagGameKind));
            for(int i=0;i<wItemCount;i++)
            {
                LOGD("%s: ==GameClient===种类消息====wSortID: %d,"
                                                           "wTypeID: %d, "
                                                           "wKindID: %d, "
                                                           "dwMaxVersion: %lu,"
                                                           "dwOnLineCount: %lu,"
                                                           "szKindName: %s,szProcessName: %s",__FUNCTION__,
                     (pGameKind+i)->wSortID,(pGameKind+i)->wTypeID,(pGameKind+i)->wKindID,
                     (pGameKind+i)->dwMaxVersion,(pGameKind+i)->dwOnLineCount,(pGameKind+i)->szKindName,
                     (pGameKind+i)->szProcessName);
            }
            memcpy(mProcessName, pGameKind[0].szProcessName,strlen(pGameKind[0].szProcessName));
            (*m_OnGameKindNotify)(pGameKind,wItemCount);
            return true;
        }
        case SUB_GP_LIST_STATION:		//站点消息
        {
            //效验参数
            assert(wDataSize % sizeof(tagGameStation) == 0);

            //处理消息
            tagGameStation* pGameStation = (tagGameStation*)pData;
            short wItemCount = (short)(wDataSize/ sizeof(tagGameStation));
            for(int i=0;i<wItemCount;i++)
            {
                LOGD("%s: ==GameClient===站点消息====wSortID: %d,"
                             "wKindID: %d, "
                             "wJoinID: %d, "
                             "wStationID: %d,"
                             "szStationName: %s",__FUNCTION__,
                     (pGameStation+i)->wSortID,(pGameStation+i)->wKindID,(pGameStation+i)->wJoinID,
                     (pGameStation+i)->wStationID,(pGameStation+i)->szStationName);
            }
            (*m_OnGameStationNotify)(pGameStation,wItemCount);
            return true;
        }
        case SUB_GP_LIST_SERVER:		//服务器房间
        {
            //效验参数
            assert(wDataSize % sizeof(tagGameServer) == 0);

            //处理消息
            tagGameServer* pGameServer = (tagGameServer*)pData;
            short wItemCount = (short)(wDataSize/ sizeof(tagGameServer));
            for(int i=0;i<wItemCount;i++)
            {
                char ip[33] = {0};
                struct in_addr addr;
                addr.s_addr = (pGameServer+i)->dwServerAddr;
                strcpy(ip,inet_ntoa(addr));
                LOGD("%s: ===GameClient====房间服务器IP： %s,  ip长度: %zu",__FUNCTION__,ip,strlen(ip));
                LOGD("%s: ==GameClient===服务器房间===="
                             "wServerType: %d,"
                             "wSortID: %d,"
                             "wKindID: %d, "
                             "wServerID: %d, "
                             "wStationID: %d,"
                             "wServerPort: %d,"
                             "dwServerAddr: %ld,"
                             "dwOnLineCount: %ld,"
                             "szServerName: %s",__FUNCTION__,
                     (pGameServer+i)->wServerType,
                     (pGameServer+i)->wSortID,(pGameServer+i)->wKindID,(pGameServer+i)->wServerID,
                     (pGameServer+i)->wStationID,(pGameServer+i)->wServerPort,(pGameServer+i)->dwServerAddr,
                     (pGameServer+i)->dwOnLineCount,(pGameServer+i)->szServerName);
            }
            (*m_OnGameServerNotify)(pGameServer,wItemCount);
            return true;
        }
        case SUB_GP_LIST_FINISH:		//列表发送完成
        {
            LOGD("%s: ====GameClient======列表发送完成============",__FUNCTION__);
            return true;
        }
        case SUB_GP_LIST_CONFIG:		//列表配置
        {
            //效验参数
            assert(wDataSize % sizeof(CMD_GP_ListConfig) == 0);

            //处理消息
            CMD_GP_ListConfig* pListConfig = (CMD_GP_ListConfig*)pData;
            short wItemCount = (short)(wDataSize/ sizeof(CMD_GP_ListConfig));
            for(int i=0;i<wItemCount;i++)
            {
                LOGD("%s: ==GameClient===列表配置====bShowOnLineCount: %d",__FUNCTION__, (pListConfig+i)->bShowOnLineCount);
            }
            return true;
        }
    }

    return true;
}

void CLogonPlaza::ClosePlaza()
{
    mIsConnect = false;
    m_pSocketEnging ->Close();
}
