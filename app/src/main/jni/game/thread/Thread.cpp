/*************************************************************************
    > File Name: Thread.cpp
    > Author: zhongjihao
    > Mail: zhongjihao100@163.com 
    > Created Time: 2015年03月27日 星期五 19时46分54秒
 ************************************************************************/
#define LOG_TAG "Thread"

#include "Thread.h"
#include "../../log.h"

Thread::Thread() : m_recvTid(-1),m_sendTid(-1),m_isRecvRuning(false),m_isSendRuning(false)
{
	LOGD("%s: =====GameClient============",__FUNCTION__);
}

Thread::~Thread()
{
	m_isRecvRuning = false;
	m_isSendRuning = false;
	m_recvTid = -1;
	m_sendTid = -1;
	LOGD("%s: =====GameClient============",__FUNCTION__);
}

void Thread::startRecvThread()
{
	LOGD("%s: =====GameClient============m_isRecvRuning: %d",__FUNCTION__,m_isRecvRuning);
	if(!m_isRecvRuning)
	{
		m_isRecvRuning = (pthread_create(&m_recvTid,NULL,OnRecvThreadFunc,this) == 0);
	}
}

void Thread::startSendThread()
{
	LOGD("%s: =====GameClient============m_isSendRuning: %d",__FUNCTION__,m_isSendRuning);
	if(!m_isSendRuning)
	{
		m_isSendRuning = (pthread_create(&m_sendTid,NULL,OnSendThreadFunc,this) == 0);
	}
}

void* Thread::OnRecvThreadFunc(void* pvoid)
{
	Thread* pThread = static_cast<Thread*>(pvoid);
	pThread ->recvRun1();
	return NULL;
}

void* Thread::OnSendThreadFunc(void* pvoid)
{
	Thread* pThread = static_cast<Thread*>(pvoid);
	pThread ->SendRun1();
	return NULL;
}

void Thread::recvRun1()
{
	LOGD("%s: =====GameClient============",__FUNCTION__);
	recvRun();
}

void Thread::SendRun1()
{
	LOGD("%s: =====GameClient============",__FUNCTION__);
	sendRun();
}
