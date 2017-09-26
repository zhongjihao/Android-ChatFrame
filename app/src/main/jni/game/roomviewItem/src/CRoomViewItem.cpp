//
// Created by SPREADTRUM\jihao.zhong on 17-8-7.
//

#define LOG_TAG "CRoomViewItem"

#include "../include/CRoomViewItem.h"
#include "../../plazaviewItem/include/CLogonPlaza.h"
#include "../../../log.h"
#include "../../network/include/CSendPacketHelper.h"
#include "../../network/include/CRecvPacketHelper.h"
#include <assert.h>

CRoomViewItem::CRoomViewItem()
{
    m_EventType = 0;
    mDataSize = 0;
    //辅助变量
    m_wReqTableID = INVALID_TABLE;
    m_wReqChairID = INVALID_CHAIR;
    m_wFindTableID = INVALID_TABLE;
    m_UserId = INVALID_USER_ID;
    //房间配置
    m_ServiceStatus = ServiceStatus_Null;
    m_pMeUserItem = new CClientUserItem;
    m_ClientUserManager = CClientUserManager::getInstance();
    m_TableFrame = new CTableFrame;
    pGameServer =  new tagGameServer;
    m_pSocketEnging = new TCPSocketEnging;
    memset(&m_ListColumnInfo,0,sizeof(m_ListColumnInfo));
    memset(&m_ServerInfo,0, sizeof(m_ServerInfo));
    memset(m_cbBuffer,0, sizeof(m_cbBuffer));
    memset(m_processName,0, sizeof(m_processName));
}

CRoomViewItem::~CRoomViewItem()
{
    LOGD("%s: ======GameClient==========" ,__FUNCTION__);
    if(m_pMeUserItem)
        delete m_pMeUserItem;
    if(m_TableFrame)
        delete m_TableFrame;
    if(pGameServer)
        delete pGameServer;
    if(m_pSocketEnging)
        delete m_pSocketEnging;
}

bool CRoomViewItem::ConnectGameRoomServer(const char* serIp,int port)
{
     mIsConnect = m_pSocketEnging ->ConnectServer(serIp,port);
     if(mIsConnect)
     {
         //启动接收线程
         startRecvThread();
         //启动发送线程
         startSendThread();
     }
    return mIsConnect;
}

void CRoomViewItem::OnExitRoomServer(int wTableID,int wChairID)
{
    CMD_GR_UserReq exitRoomReq;
    exitRoomReq.dwUserID = m_UserId;
    exitRoomReq.wTableID = wTableID;
    exitRoomReq.wChairID = wChairID;

    memcpy(m_cbBuffer,&exitRoomReq,sizeof(CMD_GR_UserReq));
    mDataSize = sizeof(exitRoomReq);
    m_EventType = EXIT_ROOM_REQ;
}

void CRoomViewItem::OnDeleteRoom()
{
    mIsConnect = false;
    delete this;
}

//发送登录包
void CRoomViewItem::LogonGameRoom(CMD_GR_LogonByUserID* pLogonByID,tagClientSerial* pClientSerial)
{
    m_UserId = pLogonByID->dwUserID;
    //登录数据包
    memcpy(m_cbBuffer,pLogonByID,sizeof(CMD_GR_LogonByUserID));

    CSendPacketHelper Packet(m_cbBuffer+sizeof(CMD_GR_LogonByUserID),SOCKET_PACKET-sizeof(CMD_GR_LogonByUserID));
    Packet.AddPacket(pClientSerial, sizeof(tagClientSerial), DTP_COMPUTER_ID);
    mDataSize = sizeof(CMD_GR_LogonByUserID)+Packet.GetDataSize();
    m_EventType = LOGON_ROOM;
}

//发送坐下请求
void CRoomViewItem::SendSitDownRequest(int wTableID,int wChairID)
{
    CMD_GR_UserReq sitDownReq;
    sitDownReq.dwUserID = m_UserId;
    sitDownReq.wTableID = wTableID;
    sitDownReq.wChairID = wChairID;

    memcpy(m_cbBuffer,&sitDownReq,sizeof(CMD_GR_UserReq));
    mDataSize = sizeof(sitDownReq);
    m_EventType = SIT_DOWN_REQ;
}

//发送站立请求
void CRoomViewItem::SendStandUpRequest(int wTableID,int wChairID)
{
    CMD_GR_UserReq standUpReq;
    standUpReq.dwUserID = m_UserId;
    standUpReq.wTableID = wTableID;
    standUpReq.wChairID = wChairID;
    memcpy(m_cbBuffer,&standUpReq,sizeof(CMD_GR_UserReq));
    mDataSize = sizeof(standUpReq);
    m_EventType = STAND_UP_REQ;
}

void CRoomViewItem::OnSetLoginFailCallback( void (* loginFail)(int errorCode,const char* errorDescribe))
{
    m_OnLogonFailNotify = loginFail;
}

void CRoomViewItem::OnSetLoginFinishCallback( void (* loginFinish)(CMD_GR_ColumnInfo* pColumnInfo,CMD_GR_ServerInfo* pServerInfo,int online))
{
    m_OnLogonFinishNotify = loginFinish;
}

void CRoomViewItem::OnSetUserComeCallback( void (* pUserCome)(tagUserData* pUserData,const char* pUnderWrite,const char* pGroupName))
{
    m_OnUserComeNotify = pUserCome;
}

void CRoomViewItem::OnSetUserStatusCallback( void (* FunPtr)(CMD_GR_UserStatus* pUserStatus))
{
    m_OnUserStatusNotify = FunPtr;
}

void CRoomViewItem::OnSetTableStatusCallback( void (* FunPtr)(CMD_GR_TableStatus* pTableStatus))
{
    m_OnTableStatusNotify = FunPtr;
}

void CRoomViewItem::OnSetTableInfoCallback( void (* FunPtr)(CMD_GR_TableInfo* pTableInfo))
{
    m_OnTableInfoNotify = FunPtr;
}

void CRoomViewItem::sendRun()
{
    while(m_isSendRuning)
    {
        switch (m_EventType)
        {
            case LOGON_ROOM:
            {
                int wDataSize = m_pSocketEnging->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID, m_cbBuffer, mDataSize);
                m_EventType = 0;
                LOGD("%s: ======GameClient==向斗地主房间发送的登陆包的大小: %d " ,__FUNCTION__,wDataSize);
                break;
            }
            case SIT_DOWN_REQ:
            {
                int wDataSize = m_pSocketEnging->SendData(MDM_GR_USER,SUB_GR_USER_SIT_REQ, m_cbBuffer, mDataSize);
                m_EventType = 0;
                LOGD("%s: ======GameClient==向斗地主房间发送坐下请求包的大小: %d " ,__FUNCTION__,wDataSize);
                break;
            }
            case STAND_UP_REQ:
            {
                int wDataSize = m_pSocketEnging->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP_REQ, m_cbBuffer, mDataSize);
                m_EventType = 0;
                LOGD("%s: ======GameClient==向斗地主房间发送站立请求包的大小: %d " ,__FUNCTION__,wDataSize);
                break;
            }
            case EXIT_ROOM_REQ:
            {
                int wDataSize = m_pSocketEnging->SendData(MDM_GR_USER,SUB_GR_USER_LEFT_ROOM_REQ, m_cbBuffer, mDataSize);
                m_EventType = 0;
                LOGD("%s: ======GameClient==向斗地主房间发送退出房间请求包的大小: %d " ,__FUNCTION__,wDataSize);
                break;
            }
        }
    }
    LOGD("%s: ======GameClient====房间发送线程退出====" ,__FUNCTION__);
}

void CRoomViewItem::recvRun()
{
    while(m_isRecvRuning)
    {
        if(mIsConnect)
        {
            //接收房间服务器返回数据
            RecvLogonRoomViewItemData(this);
        }
    }
    LOGD("%s: ======GameClient====房间接收线程退出====" ,__FUNCTION__);
}

void CRoomViewItem::RecvLogonRoomViewItemData(TcpSocketSink* pSocketSink)
{
    m_pSocketEnging ->OnSocketNotifyRead(pSocketSink);
}

bool CRoomViewItem::OnEventTCPSocketRead(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    switch(Command.wMainCmdID)
    {
        case MDM_GR_LOGON:			//登录消息
        {
            return OnSocketMainLogon(Command,pData,wDataSize);
        }
        case MDM_GR_USER:			//用户消息
        {
            return OnSocketMainUser(Command,pData,wDataSize);
        }
        case MDM_GR_INFO:			//配置信息
        {
            return OnSocketMainInfo(Command,pData,wDataSize);
        }
        case MDM_GR_STATUS:			//状态信息
        {
            return OnSocketMainStatus(Command,pData,wDataSize);
        }
        case MDM_GR_SYSTEM:			//系统消息
        {
            return OnSocketMainSystem(Command,pData,wDataSize);
        }
        case MDM_GR_SERVER_INFO:	//房间信息
        {
            return OnSocketMainServerInfo(Command,pData,wDataSize);
        }
        case MDM_GF_GAME:			//游戏消息
        case MDM_GF_FRAME:			//框架消息
        case MDM_GF_VIDEO:			//视频消息
        case MDM_GF_PRESENT:		//礼物消息
        {
            return OnSocketMainGameFrame(Command,pData,wDataSize);
        }
    }

    return true;
}

//登录消息
bool CRoomViewItem::OnSocketMainLogon(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_LOGON);
    switch(Command.wSubCmdID)
    {
        case SUB_GR_LOGON_SUCCESS:		//登录成功
        {
            //设置变量
            m_ServiceStatus = ServiceStatus_RecvConfigInfo;
            LOGD("%s: ===GameClient====游戏房间登陆成功=====",__FUNCTION__);
            return true;
        }
        case SUB_GR_LOGON_ERROR:		//登录失败
        {
            //效验参数
            CMD_GR_LogonError*  pLogonError = (CMD_GR_LogonError*)pData;
            assert(wDataSize >= sizeof(CMD_GR_LogonError)- sizeof(pLogonError->szErrorDescribe));

            //关闭连接
            mIsConnect = false;
            m_pSocketEnging->Close();
            m_ServiceStatus = ServiceStatus_NetShutDown;
            //显示消息
            int wDescribeSize = wDataSize-(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe));
            if(wDescribeSize > 0)
            {
                pLogonError->szErrorDescribe[wDescribeSize] = 0;
                LOGD("%s: =====GameClient===登陆失败====lErrorCode:%d,   错误描述 %s",__FUNCTION__, pLogonError->lErrorCode,pLogonError->szErrorDescribe);
            }
            (*m_OnLogonFailNotify)(pLogonError->lErrorCode,pLogonError->szErrorDescribe);
            return true;
        }
        case SUB_GR_LOGON_FINISH:		//登录完成
        {
            //设置界面
            if(m_TableFrame->GetChairCount() <= MAX_CHAIR_NORMAL)
            {
                m_TableFrame ->ShowUserInfo(true);
            }
            //设置变量
            m_ServiceStatus = ServiceStatus_Serviceing;

            //重入判断
            if(m_pMeUserItem == NULL) return false;
            tagUserData* pUserData = m_pMeUserItem->GetUserData();
            if(pUserData->wTableID != INVALID_TABLE)
            {
                //启动游戏
                //int iResult=StartGameClient();
            }
            (*m_OnLogonFinishNotify)(&m_ListColumnInfo,&m_ServerInfo,m_OnLineCount);
            LOGD("%s: ======GameClient====游戏房间登陆完成",__FUNCTION__);
            return true;
        }

    }
    return true;
}

//用户消息
bool CRoomViewItem::OnSocketMainUser(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_USER);
    switch (Command.wSubCmdID)
    {
        case SUB_GR_USER_COME:			//用户进入
        {
            LOGD("%s: ====GameClient====用户进入====wSubCmdID: %d" ,__FUNCTION__,SUB_GR_USER_COME);
            return OnSocketSubUserCome(Command,pData,wDataSize);
        }
        case SUB_GR_USER_STATUS:		//用户状态
        {
            LOGD("%s: =====GameClient=====用户状态=====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_STATUS);
            return OnSocketSubStatus(Command,pData,wDataSize);
        }
        case SUB_GR_USER_SCORE:			//用户分数
        {
            LOGD("%s: =====GameClient===用户分数====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_SCORE);
            //	return OnSocketSubScore(Command,pData,wDataSize);
        }
        case SUB_GR_USER_RIGHT:			//用户权限
        {
            LOGD("%s: =====GameClient=====用户权限====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_RIGHT);
            //	return OnSocketSubRight(Command,pData,wDataSize);
        }
        case SUB_GR_MEMBER_ORDER:		//会员等级
        {
            LOGD("%s: =====GameClient====会员等级=====wSubCmdID: %d",__FUNCTION__,SUB_GR_MEMBER_ORDER);
            //	return OnSocketSubMemberOrder(Command,pData,wDataSize);
        }
        case SUB_GR_SIT_FAILED:			//坐下失败
        {
            LOGD("%s: =====GameClient===坐下失败===wSubCmdID: %d",__FUNCTION__, SUB_GR_SIT_FAILED);
            //	return OnSocketSubSitFailed(Command,pData,wDataSize);
        }
        case SUB_GR_USER_CHAT:			//用户聊天
        {
            LOGD("%s: =====GameClient====用户聊天====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_CHAT);
            //	return OnSocketSubChat(Command,pData,wDataSize);
        }
        case SUB_GR_USER_WISPER:		//用户私语
        {
            LOGD("%s: =====GameClient====用户私语=====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_WISPER);
            //	return OnSocketSubWisper(Command,pData,wDataSize);
        }
        case SUB_GR_USER_INVITE:		//邀请玩家
        {
            LOGD("%s: =====GameClient====邀请玩家=====wSubCmdID: %d",__FUNCTION__,SUB_GR_USER_INVITE);
            //	return OnSocketSubUserInvite(Command,pData,wDataSize);
        }
    }

    return true;
}

//用户状态
bool CRoomViewItem::OnSocketSubStatus(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    //效验参数
    assert(Command.wMainCmdID == MDM_GR_USER);
    assert(Command.wSubCmdID == SUB_GR_USER_STATUS);
    if(wDataSize < sizeof(CMD_GR_UserStatus)) return false;

    //处理数据
    CMD_GR_UserStatus* pUserStatus = (CMD_GR_UserStatus*)pData;
    LOGD("%s: =====GameClient=======UserId: %lu",__FUNCTION__,pUserStatus->dwUserID);
    IClientUserItem* pIUserItem = m_ClientUserManager->SearchUserByUserID(pUserStatus->dwUserID);
    if(pIUserItem == NULL) return true;

    //定义变量
    tagUserData* pUserData = pIUserItem->GetUserData();
    tagUserData* pMeUserData = m_pMeUserItem ->GetUserData();
    int wNowTableID = pUserStatus->wTableID;
    int wLastTableID = pUserData->wTableID;
    int wNowChairID = pUserStatus->wChairID;
    int wLastChairID = pUserData->wChairID;
    unsigned char cbNowStatus = pUserStatus->cbUserStatus;
    unsigned char cbLastStatus = pUserData->cbUserStatus;

    //清理旧状态
    if((wLastTableID != INVALID_TABLE) && ((wNowTableID != wLastTableID)||(wNowChairID != wLastChairID)))
    {
        if(wLastChairID == INVALID_CHAIR) return true;
        IClientUserItem* pITableUserItem = m_TableFrame->GetUserInfo(wLastTableID,wLastChairID);

        if(pITableUserItem->GetUserData()->dwUserID == pIUserItem->GetUserData()->dwUserID)
            m_TableFrame->SetUserInfo(wLastTableID,wLastChairID,NULL);
    }

    //用户离开
    if(cbNowStatus == US_NULL)
    {
        //通知游戏
        if((pMeUserData->wTableID != INVALID_TABLE) && (pMeUserData->wTableID == wLastTableID))
        {
            IPC_UserStatus UserStatus;
            UserStatus.dwUserID = pUserData->dwUserID;
            UserStatus.cbUserStatus = pUserData->cbUserStatus;

            m_pSocketEnging->SendData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus, sizeof(IPC_UserStatus));
        }

        //删除用户
        m_ClientUserManager->DeleteUserItem(pIUserItem);

        //更新人数
        int dwOnLineCount = m_ClientUserManager->GetOnLineCount();
        LOGD("%s: =====GameClient====dwOnLineCount: %d",__FUNCTION__,dwOnLineCount);
    }

    (*m_OnUserStatusNotify)(pUserStatus);
    return true;
}

//配置消息
bool CRoomViewItem::OnSocketMainInfo(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_INFO);
    switch (Command.wSubCmdID)
    {
        case SUB_GR_SERVER_INFO:	//房间信息
        {
            LOGD("%s: ==1===GameClient====房间信息========wDataSize: %d",__FUNCTION__,wDataSize);
            //效验数据
            if(wDataSize < sizeof(CMD_GR_ServerInfo)) return false;

            //消息处理
            CMD_GR_ServerInfo* pServerInfo = (CMD_GR_ServerInfo*)pData;

            //设置变量
            memcpy(&m_ServerInfo,pServerInfo,sizeof(CMD_GR_ServerInfo));

            char ip[33] = {0};
            struct in_addr addr;
            addr.s_addr = m_ServerInfo.dwVideoAddr;
            strcpy(ip,inet_ntoa(addr));
            LOGD("%s: ==2===GameClient====房间信息========videoIp: %s",__FUNCTION__,ip);

            //获取信息
            strcpy(m_processName,CLogonPlaza::getInstance()->GetProcessName());
            LOGD("%s: ==3===GameClient====房间信息========m_processName: %s",__FUNCTION__,m_processName);

            m_TableFrame->InitTableFrame(m_ServerInfo.wTableCount,m_ServerInfo.wChairCount, m_ServerInfo.cbHideUserInfo);
            LOGD("%s: ==4===GameClient====房间信息========"
                         "m_processName: %s, wKindID: %d  wTableCount: %d   "
                         "wChairCount: %d  dwVideoAddr: %d  wGameGenre: %d  cbHideUserInfo: %c",__FUNCTION__,m_processName,
                 m_ServerInfo.wKindID,m_ServerInfo.wTableCount,m_ServerInfo.wChairCount,m_ServerInfo.dwVideoAddr,m_ServerInfo.wGameGenre,
                 m_ServerInfo.cbHideUserInfo);
            return true;
        }
        case SUB_GR_COLUMN_INFO:	//列表解释
        {
            //变量定义
            CMD_GR_ColumnInfo* pColumnInfo = (CMD_GR_ColumnInfo*)pData;
            short wHeadSize = sizeof(pColumnInfo->wColumnCount);
            LOGD("%s: ===1===GameClient=====列表解释=====wDataSize: %d",__FUNCTION__,wDataSize);
            //效验参数
            if(wDataSize < wHeadSize) return false;
            if(sizeof(CMD_GR_ColumnInfo) != wDataSize) return false;
            //设置列表
            memcpy(&m_ListColumnInfo, pColumnInfo, sizeof(CMD_GR_ColumnInfo));
            for(int i=0;i<m_ListColumnInfo.wColumnCount;i++)
            {
                LOGD("%s: ====2===GameClient=====列表解释=====wColumnCount: %d , cbDataDescribe : %c, szColumnName: %s, wColumnWidth: %d, nFormat: %d",__FUNCTION__,
                     pColumnInfo->wColumnCount,(pColumnInfo->pColumnItem+i)->cbDataDescribe,
                     (pColumnInfo->pColumnItem+i)->szColumnName, (pColumnInfo->pColumnItem+i)->wColumnWidth,
                     (pColumnInfo->pColumnItem+i)->nFormat);
            }

            return true;
        }
        case SUB_GR_CONFIG_FINISH:	//配置完成
        {
            //设置变量
            m_ServiceStatus = ServiceStatus_RecvRoomInfo;
            LOGD("%s:=====GameClient====配置完成: %d" ,__FUNCTION__,SUB_GR_CONFIG_FINISH);
            return true;
        }
    }
    return true;
}

//状态消息
bool CRoomViewItem::OnSocketMainStatus(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_STATUS);
    switch (Command.wSubCmdID)
    {
        case SUB_GR_TABLE_INFO:		//桌子信息
        {
            //变量定义
            CMD_GR_TableInfo* pTableInfo = (CMD_GR_TableInfo*)pData;
            short wHeadSize = sizeof(pTableInfo->wTableCount);
            //效验数据
            if(wDataSize < wHeadSize) return false;
            if(sizeof(CMD_GR_TableInfo) != wDataSize) return false;

            LOGD("%s:======GameClient======桌子信息=====wTableCount: %d" ,__FUNCTION__, pTableInfo->wTableCount);
            for(int i=0;i<pTableInfo->wTableCount;i++)
            {
                LOGD("%s:======GameClient======桌子信息=====bTableLock: %d   bPlayStatus: %d" ,__FUNCTION__,
                     (pTableInfo->pTableStatus+i)->bTableLock, (pTableInfo->pTableStatus+i)->bPlayStatus);
            }
            (*m_OnTableInfoNotify)(pTableInfo);
            return true;
        }
        case SUB_GR_TABLE_STATUS:	//桌子状态
        {
            //效验数据
            assert(wDataSize >= sizeof(CMD_GR_TableStatus));
            //消息处理
            CMD_GR_TableStatus* pTableStatus = (CMD_GR_TableStatus*)pData;
            if(pTableStatus->wTableID >= m_TableFrame->GetTableCount()) return false;

            //变量定义
            IClientUserItem* pIUserItem = NULL;
            tagUserData* pUserData = NULL;
            bool bPlayStatustemp = m_TableFrame->QueryPlayFlag(pTableStatus->wTableID);
            unsigned char bPlayStatus =0;
            bPlayStatus = bPlayStatustemp ? 1 : 0;
            //设置用户
            if(bPlayStatus != pTableStatus->bPlayStatus)
            {
                unsigned char cbUserStatus = pTableStatus->bPlayStatus ? US_PLAY : US_SIT;
                for(short i=0;i<m_TableFrame->GetChairCount();i++)
                {
                    pIUserItem = m_TableFrame->GetUserInfo(pTableStatus->wTableID,i);
                    if(pIUserItem!=NULL)
                    {
                        pUserData = pIUserItem->GetUserData();
                        if(pUserData->cbUserStatus != US_OFFLINE)
                        {
                            pUserData->cbUserStatus = cbUserStatus;
                            OnUserItemUpdate(pIUserItem);
                        }
                    }
                }
            }
            //设置桌子
            m_TableFrame->SetPassFlag(pTableStatus->wTableID,pTableStatus->bTableLock);
            m_TableFrame->SetPlayFlag(pTableStatus->wTableID,pTableStatus->bPlayStatus);

            //通知游戏
            if(bPlayStatus != pTableStatus->bPlayStatus)
            {
                tagUserData* pMeUserData = m_pMeUserItem->GetUserData();
                if(pMeUserData->wTableID == pTableStatus->wTableID)
                {
                    short wSubCmdID = pTableStatus->bPlayStatus ? IPC_SUB_GAME_START : IPC_SUB_GAME_FINISH;
                    m_pSocketEnging->SendData(IPC_MAIN_USER,wSubCmdID,NULL,0);
                }
            }

            LOGD("%s:====GameClient====桌子状态=====wTableID: %d, bTableLock : %d, bPlayStatus : %d",__FUNCTION__,
                             pTableStatus->wTableID,pTableStatus->bTableLock,pTableStatus->bPlayStatus);
            (*m_OnTableStatusNotify)(pTableStatus);
            return true;
        }
    }
    return true;
}

//系统消息
bool CRoomViewItem::OnSocketMainSystem(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_SYSTEM);
    switch (Command.wSubCmdID)
    {
        case SUB_GR_MESSAGE:		//系统消息
        {
            LOGD("%s:=======GameClient====系统消息: %d",__FUNCTION__,SUB_GR_MESSAGE);
            return true;
        }
    }
    return true;
}

//房间消息
bool CRoomViewItem::OnSocketMainServerInfo(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    assert(Command.wMainCmdID == MDM_GR_SERVER_INFO);
    switch (Command.wSubCmdID)
    {
        case SUB_GR_ONLINE_COUNT_INFO:		//在线信息
        {
            m_OnLineCount = *(int*)pData;
            LOGD("%s:=====GameClient======在线信息=======房间在线人数： %d",__FUNCTION__,m_OnLineCount);
            return true;
        }
    }
    return true;
}

//用户进入
bool CRoomViewItem::OnSocketSubUserCome(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    //效验参数
    assert(Command.wMainCmdID == MDM_GR_USER);
    assert(Command.wSubCmdID == SUB_GR_USER_COME);

    if(wDataSize < sizeof(tagUserInfoHead)) return false;

    //读取基本信息
    tagUserData UserData;
    memset(&UserData,0, sizeof(UserData));
    tagUserInfoHead* pUserInfoHead = (tagUserInfoHead*)pData;
    //读取信息
    UserData.dwUserID = pUserInfoHead->dwUserID;
    UserData.wTableID = pUserInfoHead->wTableID;
    UserData.wChairID = pUserInfoHead->wChairID;
    UserData.cbUserStatus = pUserInfoHead->cbUserStatus;
    UserData.cbIsOut = false;
    UserData.wMatchRank = 100;
    UserData.dwUserRight = pUserInfoHead->dwUserRight;
    UserData.dwMasterRight = pUserInfoHead->dwMasterRight;

    //管理判断
    if((pUserInfoHead->dwUserID == m_UserId)&&(pUserInfoHead->cbMasterOrder>=2))
    {
        m_ServerInfo.cbHideUserInfo = 0;
    }

    //读取信息
    if(m_ServerInfo.cbHideUserInfo == 0 || (pUserInfoHead->dwUserID == m_UserId))
    {
        UserData.wFaceID = pUserInfoHead->wFaceID;
        UserData.dwCustomFaceVer = pUserInfoHead->dwCustomFaceVer;
        UserData.cbGender = pUserInfoHead->cbGender;
        UserData.cbMemberOrder = pUserInfoHead->cbMemberOrder;
        UserData.cbMasterOrder = pUserInfoHead->cbMasterOrder;
        UserData.dwGameID = pUserInfoHead->dwGameID;
        UserData.dwGroupID = pUserInfoHead->dwGroupID;
        UserData.lLoveliness = pUserInfoHead->lLoveliness;
        UserData.lScore = pUserInfoHead->UserScoreInfo.lScore;
        UserData.lGameGold = pUserInfoHead->UserScoreInfo.lGameGold;
        UserData.lInsureScore = pUserInfoHead->UserScoreInfo.lInsureScore;
        UserData.lWinCount = pUserInfoHead->UserScoreInfo.lWinCount;
        UserData.lLostCount = pUserInfoHead->UserScoreInfo.lLostCount;
        UserData.lDrawCount = pUserInfoHead->UserScoreInfo.lDrawCount;
        UserData.lFleeCount = pUserInfoHead->UserScoreInfo.lFleeCount;
        UserData.lExperience = pUserInfoHead->UserScoreInfo.lExperience;
    }

    //读取扩展信息
    void* pDataBuffer = NULL;
    tagDataDescribe DataDescribe;
    void* pUserCome = NULL;
    pUserCome = (void*)(pUserInfoHead+1);

    CRecvPacketHelper RecvPacket(pUserCome,wDataSize- sizeof(tagUserInfoHead));
    char szGroupName[GROUP_LEN];
    char szUnderWrite[UNDER_WRITE_LEN];

    while(true)
    {
        pDataBuffer = RecvPacket.GetData(DataDescribe);
        if(DataDescribe.wDataDescribe == 0) break;
        switch(DataDescribe.wDataDescribe)
        {
            case DTP_USER_ACCOUNTS:		//用户帐户
            {
                if(m_ServerInfo.cbHideUserInfo == 0 ||(pUserInfoHead->dwUserID == m_UserId))
                {
                    assert(pDataBuffer != NULL);
                    assert(DataDescribe.wDataSize < NAME_LEN);
                    assert(DataDescribe.wDataSize >0);
                    memcpy(UserData.szName,pDataBuffer,DataDescribe.wDataSize);
                    LOGD("%s: ==GameClient=====用户帐户: %s",__FUNCTION__,UserData.szName);
                }
                else
                {
                    memcpy(UserData.szName,"游戏用户", strlen("游戏用户"));
                }
                break;
            }
            case DTP_USER_PASS:		//用户密码
            {
                if(m_ServerInfo.cbHideUserInfo == 0 ||(pUserInfoHead->dwUserID == m_UserId))
                {
                    assert(pDataBuffer != NULL);
                    assert(DataDescribe.wDataSize < PASS_LEN);
                    assert(DataDescribe.wDataSize >0);
                    memcpy(UserData.szPassword,pDataBuffer,DataDescribe.wDataSize);
                    LOGD("%s: ==GameClient=====用户密码: %s",__FUNCTION__,UserData.szPassword);
                }
                else
                {
                    memcpy(UserData.szPassword,"111111", strlen("111111"));
                }
                break;
            }
            case DTP_USER_PHONE:		//用户手机号
            {
                if(m_ServerInfo.cbHideUserInfo == 0 ||(pUserInfoHead->dwUserID == m_UserId))
                {
                    assert(pDataBuffer != NULL);
                    assert(DataDescribe.wDataSize < PHONE_LEN);
                    assert(DataDescribe.wDataSize >0);
                    memcpy(UserData.szPhone,pDataBuffer,DataDescribe.wDataSize);
                    LOGD("%s: ==GameClient=====用户手机号: %s",__FUNCTION__,UserData.szPhone);
                }
                else
                {
                    memcpy(UserData.szPhone,"18626455927", strlen("18626455927"));
                }
                break;
            }
            case DTP_UNDER_WRITE:		//个性签名
            {
                if(m_ServerInfo.cbHideUserInfo == 0||(pUserInfoHead->dwUserID == m_UserId))
                {
                    assert(pDataBuffer != NULL);
                    assert(DataDescribe.wDataSize < UNDER_WRITE_LEN);
                    assert(DataDescribe.wDataSize >0);

                    memcpy(szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
                    szUnderWrite[DataDescribe.wDataSize] = 0;
                    LOGD("%s: ==GameClient=====个性签名: %s",__FUNCTION__,szUnderWrite);
                }
                break;
            }
            case DTP_USER_GROUP_NAME:	//用户社团
            {
                if(m_ServerInfo.cbHideUserInfo == 0 ||(pUserInfoHead->dwUserID == m_UserId))
                {
                    assert(pDataBuffer != NULL);
                    assert(DataDescribe.wDataSize > 0);
                    assert(DataDescribe.wDataSize<= GROUP_LEN);

                    memcpy(szGroupName,pDataBuffer,DataDescribe.wDataSize);
                    szGroupName[DataDescribe.wDataSize] = 0;
                    LOGD("%s: ==GameClient=====社团名字: %s",__FUNCTION__,szGroupName);
                }
                break;
            }
        }
    }
    //查找用户
    IClientUserItem*  pIUserItem = m_ClientUserManager->SearchUserByUserID(UserData.dwUserID);
    LOGD("%s:===1====GameClient=====dwUserID：%lu",__FUNCTION__,UserData.dwUserID);
    if(pIUserItem == NULL)
    {
        LOGD("%s:===2====GameClient=====dwUserID：%lu",__FUNCTION__,UserData.dwUserID);
        pIUserItem = m_ClientUserManager->ActiveUserItem(&UserData);
    }
    else
        OnUserItemUpdate(pIUserItem);

    //加入用户信息
    if(pIUserItem == NULL) return false;
    if(pIUserItem != NULL)
    {
        //判断自己
        if(m_pMeUserItem == NULL) m_pMeUserItem = pIUserItem;
        //设置界面
        unsigned char cbUserStatus = UserData.cbUserStatus;
        if((cbUserStatus >= US_SIT) && (cbUserStatus != US_LOOKON))
        {
            //设置用户信息
            m_TableFrame->SetUserInfo(UserData.wTableID,UserData.wChairID,pIUserItem);
        }
    }
    LOGD("%s:===3====GameClient=====dwUserID：%lu",__FUNCTION__,UserData.dwUserID);
    //更新人数
    int dwOnLineCount = m_ClientUserManager->GetOnLineCount();
    LOGD("%s:=======GameClient=====在线人数：%d",__FUNCTION__,dwOnLineCount);
    (*m_OnUserComeNotify)(&UserData,szUnderWrite,szGroupName);
    return true;
}

//游戏消息
bool CRoomViewItem::OnSocketMainGameFrame(CMD_Command Command, void* pData, unsigned short wDataSize)
{
    //效验数据
    assert(wDataSize < SOCKET_PACKET);
    LOGD("%s: =======GameClient====游戏消息====wMainCmdID:  %d     wSubCmdID: %d",__FUNCTION__,Command.wMainCmdID,Command.wSubCmdID);
    return true;
}

//用户更新
void CRoomViewItem::OnUserItemUpdate(IClientUserItem* pIUserItem)
{
    bool ret = m_ClientUserManager->UpdateUserItem(pIUserItem);
    LOGD("%s:=======GameClient=====用户更新：%d",__FUNCTION__,ret);
    return;
}

