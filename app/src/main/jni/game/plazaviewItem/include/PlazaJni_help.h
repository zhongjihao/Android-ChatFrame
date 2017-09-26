//
// Created by SPREADTRUM\jihao.zhong on 17-8-2.
//

#ifndef CHATFRAME_PLAZAJNI_HELP_H
#define CHATFRAME_PLAZAJNI_HELP_H


#include <jni.h>

//登陆大厅成功本地结构体
struct CMD_GP_LogonSuccessNativeJNI{
    jclass     logonSuccess_cls;
    jmethodID  constructionID;
    jmethodID  setwFaceID;
    jmethodID  setCbGenderID;
    jmethodID  setCbMemberID;
    jmethodID  setDwUserID;
    jmethodID  setDwGameID;
    jmethodID  setDwExperienceID;
    jmethodID  setlLovelinessID;
    jmethodID  setlGoldCoinID;
    jmethodID  setlScoreID;
    jmethodID  setDwCustomFaceVerID;
};

//注册大厅成功本地结构体
struct CMD_GP_RegisterSuccessNativeJNI{
    jclass     registerSuccess_cls;
    jmethodID  constructionID;
    jmethodID  setwFaceID;
    jmethodID  setCbGenderID;
    jmethodID  setCbMemberID;
    jmethodID  setDwUserID;
    jmethodID  setDwGameID;
    jmethodID  setDwExperienceID;
    jmethodID  setlLovelinessID;
    jmethodID  setlGoldCoinID;
    jmethodID  setlScoreID;
    jmethodID  setSzAcccountID;
    jmethodID  setSzPasswordID;
    jmethodID  setSzPhoneID;
};

//游戏类型本地结构体
struct tagGameTypeNativeJNI{
    jclass     gameType_cls;
    jmethodID  constructionID;
    jmethodID  setwSortID;
    jmethodID  setwTypeID;
    jmethodID  setSzTypeName;
};

//游戏种类本地结构体
struct tagGameKindNativeJNI{
    jclass     gameKind_cls;
    jmethodID  constructionID;
    jmethodID  setwSortID;
    jmethodID  setwTypeID;
    jmethodID  setwKindID;
    jmethodID  setDwMaxVersionID;
    jmethodID  setDwOnLineCountID;
    jmethodID  setSzKindNameID;
    jmethodID  setSzProcessNameID;
};

//游戏站点本地结构体
struct tagGameStationNativeJNI{
    jclass     gameStation_cls;
    jmethodID  constructionID;
    jmethodID  setwSortID;
    jmethodID  setwKindID;
    jmethodID  setwJoinID;
    jmethodID  setwStationID;
    jmethodID  setSzStationNameID;
};

//游戏房间列表本地结构体
struct tagGameServerNativeJNI{
    jclass     gameServer_cls;
    jmethodID  constructionID;
    jmethodID  setwServerTypeID;
    jmethodID  setwSortID;
    jmethodID  setwKindID;
    jmethodID  setwServerID;
    jmethodID  setwStationID;
    jmethodID  setwServerPortID;
    jmethodID  setDwServerAddrID;
    jmethodID  setDwOnLineCountID;
    jmethodID  setSzServerNameID;
};

void initLogonSuccessNativeJNI(JNIEnv* env,const char* className);
void initRegisterSuccessNativeJNI(JNIEnv* env,const char* className);
void initGameTypeNativeJNI(JNIEnv* env,const char* className);
void initGameKindNativeJNI(JNIEnv* env,const char* className);
void initGameStationNativeJNI(JNIEnv* env,const char* className);
void initGameServerNativeJNI(JNIEnv* env,const char* className);

#endif //CHATFRAME_PLAZAJNI_HELP_H
