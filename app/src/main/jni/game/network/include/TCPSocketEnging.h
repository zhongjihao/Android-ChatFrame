//
// Created by SPREADTRUM\jihao.zhong on 17-6-15.
//

#ifndef CHATFRAME_TCPSOCKETENGING_H
#define CHATFRAME_TCPSOCKETENGING_H

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include "../../datatype/GameData.h"
#include "../include/TcpSocketSink.h"


class TCPSocketEnging
{
private:
    int m_serFd;
    int m_wRecvSize;
    unsigned char m_cbRecvBuf[SOCKET_BUFFER];
    static pthread_mutex_t s_writeMutex;
private:
    int  OnSendCompleted(const void* data, size_t wSenddata);
    void OnRecvCompleted(TcpSocketSink* m_pSocketSink);
    int  blockingWrite(const void *buffer, size_t len);
public:
    TCPSocketEnging();
    ~TCPSocketEnging();
    bool ConnectServer(const char* serIP,int port); //连接服务器
    void OnSocketNotifyRead(TcpSocketSink* m_pSocketSink);   //接收数据
    int SendData(short wMainCmdID, short wSubCmdID,void* pData, size_t wDataSize);    //发送数据
    void Close();    //关闭Socket
};

#endif //CHATFRAME_TCPSOCKETENGING_H
