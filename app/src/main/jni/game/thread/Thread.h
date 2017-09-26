/*************************************************************************
    > File Name: Thread.h
    > Author: zhongjihao
    > Mail: zhongjihao100@163.com 
    > Created Time: 2015年03月27日 星期五 18时41分50秒
 ************************************************************************/

#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>


class Thread
{
private:
	pthread_t       m_recvTid;         //接收线程ID
	pthread_t       m_sendTid;         //发送线程ID

protected:
	volatile bool            m_isRecvRuning;
	volatile bool            m_isSendRuning;

private:
	void recvRun1();
	void SendRun1(); //内部函数
	static void* OnRecvThreadFunc(void* pvoid); //接收线程运行的函数指针
	static void* OnSendThreadFunc(void* pvoid); //发送线程运行的函数指针
public:
	Thread();
	virtual ~Thread();
	virtual void recvRun() = 0;            //接收线程运行实体
	virtual void sendRun() = 0;            //发送线程运行实体

protected:
	void startRecvThread();                //启动接收线程
	void startSendThread();                //启动发送线程
};

#endif
