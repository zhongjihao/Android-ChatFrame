/*************************************************************************
    > File Name: CClientUserManager.h
    > Author: zhongjihao
    > Mail: zhongjihao100@163.com 
    > Created Time: Sat 01 Jul 2017 03:07:12 PM CST
 ************************************************************************/

#ifndef CCLIENT_USER_MANAGER_H
#define CCLIENT_USER_MANAGER_H

#include <vector>
#include "IClientUserManager.h"
#include "CClientUserItem.h"

//全局用户管理类
class CClientUserManager : public IClientUserManager
{
private:
	static CClientUserManager*                m_pInstance;
	//变量定义
	std::vector<CClientUserItem*>             m_UserItemActive;                 //活动数组
	std::vector<CClientUserItem*>             m_UserItemStorage;                //存储数组

private:
	CClientUserManager();
	CClientUserManager(const CClientUserManager&);
	CClientUserManager& operator=(const CClientUserManager&);
	~CClientUserManager();
public:
	 static CClientUserManager* getInstance();

public:
	 IClientUserItem* ActiveUserItem(tagUserData* UserData);
	 bool DeleteUserItem(IClientUserItem* pIUserItem);
	 bool UpdateUserItem(IClientUserItem* pIUserItem);
	 bool UpdateUserItemScore(IClientUserItem* pIUserItem, tagUserScore* pUserScore);
	 bool UpdateUserItemStatus(IClientUserItem* pIUserItem, tagUserStatus* pUserStatus);
     int GetOnLineCount();
     IClientUserItem* EnumUserItem(unsigned short wEnumIndex);
     IClientUserItem* SearchUserByUserID(unsigned int dwUserID);
	 IClientUserItem* SearchUserByPhone(const char* phone);
     IClientUserItem* SearchUserByGameID(unsigned int dwGameID);
     void Release();
};

#endif

