//
// Created by SPREADTRUM\jihao.zhong on 17-6-15.
//

#define LOG_TAG "TCPSocketEnging"

#include "../include/TCPSocketEnging.h"
#include "../../../log.h"

pthread_mutex_t TCPSocketEnging::s_writeMutex = PTHREAD_MUTEX_INITIALIZER;

TCPSocketEnging::TCPSocketEnging() : m_serFd(-1),m_wRecvSize(0)
{
    memset(m_cbRecvBuf,0, sizeof(m_cbRecvBuf));
}

TCPSocketEnging::~TCPSocketEnging()
{
    Close();
}

bool TCPSocketEnging::ConnectServer(const char* serIP,int port)
{
    m_serFd = socket(AF_INET,SOCK_STREAM,6);
    if(m_serFd == -1)
    {
        LOGE("%s: =====GameClient===建立socket失败========%s",__FUNCTION__,strerror(errno));
        return false;
    }
    LOGD("%s: ===GameClient=====建立socket成功=======",__FUNCTION__);
    struct sockaddr_in dr;
    bzero(&dr,sizeof(dr));
    dr.sin_family = AF_INET;
    dr.sin_port = htons(port);
    inet_aton(serIP,&dr.sin_addr);
    int ret = connect(m_serFd,(struct sockaddr*)&dr,sizeof(dr));
    if(ret == -1)
    {
        LOGE("%s: ======GameClient====connect连接失败=======%s",__FUNCTION__,strerror(errno));
        close(m_serFd);
        m_serFd = -1;
        return false;
    }
    LOGD("%s: ====GameClient===connect连接成功===========ip: %s,port: %d    m_serFd: %d",__FUNCTION__,serIP,port,m_serFd);
    return true;
}

int TCPSocketEnging::OnSendCompleted(const void* data, size_t wSenddata)
{
    uint32_t header;
    int ret;
    pthread_mutex_t * writeMutexHook = &s_writeMutex;

    if (m_serFd < 0) {
        return -1;
    }

    if (wSenddata > SOCKET_BUFFER) {
        LOGE("%s: ====GameClient=====TCP: packet larger than %u (%u)",__FUNCTION__,SOCKET_BUFFER, (unsigned int )wSenddata);
        return  -1;
    }

    pthread_mutex_lock(writeMutexHook);
    //发送数据长度
    header = htonl(wSenddata);
    LOGD("%s: ====GameClient===要发送的字节数: %zu,   header:%u",__FUNCTION__,wSenddata,header);
    ret = blockingWrite((void *)&header, sizeof(header));

    if (ret < 0) {
        pthread_mutex_unlock(writeMutexHook);
        return ret;
    }

    //发送数据
    ret = blockingWrite(data, wSenddata);

    if (ret < 0) {
        pthread_mutex_unlock(writeMutexHook);
        return ret;
    }

    pthread_mutex_unlock(writeMutexHook);
    LOGD("%s: ======GameClient========ret: %d,  wSenddata: %zu",__FUNCTION__,ret,wSenddata);
    return wSenddata;
}

int TCPSocketEnging::blockingWrite(const void *buffer, size_t len)
{
    size_t writeOffset = 0;
    const uint8_t *toWrite;

    toWrite = (const uint8_t *)buffer;

    while(writeOffset < len)
    {
        ssize_t written;
        do {
            written = write(m_serFd, toWrite + writeOffset,len - writeOffset);
        } while (written < 0 && ((errno == EINTR) || (errno == EAGAIN)));

        if (written >= 0) {
            writeOffset += written;
        } else { // written < 0
            LOGE ("%s: =====GameClient====unexpected error on write errno:%d", __FUNCTION__,errno);
            close(m_serFd);
            m_serFd = -1;
            return -1;
        }
    }

    LOGD("%s: =====GameClient=====Request bytes written:%zu", __FUNCTION__,writeOffset);
    return writeOffset;
}


void TCPSocketEnging::OnRecvCompleted(TcpSocketSink* m_pSocketSink)
{
    int ret = 0;
    //接收数据
    if( m_serFd == -1)
        return;
    ret = recv(m_serFd,m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);
    if(ret == -1 || ret == 0)
    {
        LOGE("%s: ===GameClient====recv数据===%s",__FUNCTION__,(ret == -1) ? strerror(errno) : "对端socket关闭");
        close(m_serFd);
        m_serFd = -1;
        return;
    }

    //接收完成
    m_wRecvSize += ret;
    unsigned char cbBuffer[SOCKET_BUFFER];
    memset(cbBuffer,0,SOCKET_BUFFER);
    CMD_Head * pHead = (CMD_Head *)m_cbRecvBuf;

    //处理数据
    try
    {
        while(m_wRecvSize >= sizeof(CMD_Head))
        {
            //效验数据
            unsigned short wPacketSize = pHead->CmdInfo.wPacketSize;
            if (wPacketSize > SOCKET_BUFFER) throw "数据包超长";
            if (wPacketSize < sizeof(CMD_Head)) throw "数据包非法";
            if (pHead->CmdInfo.cbVersion != SOCKET_VER) throw "数据包版本错误";
            if (m_wRecvSize < wPacketSize) break;

            memcpy(cbBuffer,m_cbRecvBuf,wPacketSize);

            //解释数据
            unsigned short wDataSize = wPacketSize - sizeof(CMD_Head);
            void * pDataBuffer = cbBuffer + sizeof(CMD_Head);
            CMD_Command Command = ((CMD_Head *)cbBuffer)->CommandInfo;
            LOGD("%s: ===GameClient====主命令: %d   ,子命令: %d",__FUNCTION__,Command.wMainCmdID,Command.wSubCmdID);

            //删除缓存数据
            m_wRecvSize -= wPacketSize;
            memcpy(m_cbRecvBuf,m_cbRecvBuf+wPacketSize,m_wRecvSize);

            //内核命令
            if(Command.wMainCmdID == MDM_KN_COMMAND)
            {
                switch (Command.wSubCmdID)
                {
                    case SUB_KN_DETECT_SOCKET:    //网络检测
                    {
                        SendData(MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
                        break;
                    }
                    default: throw "非法命令码";
                }
                continue;
            }

            //消息处理
            m_pSocketSink->OnEventTCPSocketRead(Command,pDataBuffer,wDataSize);
        }
    }
    catch (const char* msg)
    {
        LOGE("%s: ====GameClient===OnRecvCompleted===异常: %s",__FUNCTION__,msg);
        close(m_serFd);
        m_serFd = -1;
    }
    catch(...)
    {
        close(m_serFd);
        m_serFd = -1;
    }

    return;
}

void TCPSocketEnging::OnSocketNotifyRead(TcpSocketSink* m_pSocketSink)
{
    OnRecvCompleted(m_pSocketSink);
}

int TCPSocketEnging::SendData(short wMainCmdID, short wSubCmdID,void* pData, size_t wDataSize)
{
    if(wDataSize + sizeof(CMD_Head) > SOCKET_BUFFER)
    {
        LOGE("%s: ===GameClient====SendData===发送数据太大======",__FUNCTION__);
        return 0;
    }
    //构造数据
    char cbDataBuffer[SOCKET_BUFFER];
    memset(cbDataBuffer,0,SOCKET_BUFFER);
    CMD_Head  pHead;
    pHead.CmdInfo.cbVersion = SOCKET_VER;
    pHead.CmdInfo.cbCheckCode = 0;
    pHead.CmdInfo.wPacketSize = sizeof(CMD_Head)+wDataSize;
    pHead.CommandInfo.wMainCmdID = wMainCmdID;
    pHead.CommandInfo.wSubCmdID = wSubCmdID;

    memcpy(cbDataBuffer,&pHead, sizeof(CMD_Head));
    if(wDataSize > 0)
    {
        if(pData != NULL)
        {
            memcpy(cbDataBuffer + sizeof(CMD_Head),pData,wDataSize);
        }
    }

    //发送数据
    return OnSendCompleted(cbDataBuffer, sizeof(CMD_Head)+wDataSize);
}

void TCPSocketEnging::Close()
{
    if(m_serFd != -1)
    {
        LOGD("%s: ===GameClient====关闭房间socket： %d=====",__FUNCTION__,m_serFd);
        close(m_serFd);
        m_serFd = -1;
    }
}

