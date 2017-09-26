//
// Created by SPREADTRUM\jihao.zhong on 17-9-2.
//

#ifndef CHATFRAME_ROOMJNI_HELP_H
#define CHATFRAME_ROOMJNI_HELP_H

#include <jni.h>

//列表描述信息本地结构体
struct CMD_GR_ColumnInfoNativeJNI{
    jclass     columnInfo_cls;
    jmethodID  constructionID;
    jmethodID  setwColumnCountID;
    jmethodID  setpColumnItemID;
};

//列表项描述本地结构体
struct TagColumnItemNativeJNI{
    jclass     tagColumnItem_cls;
    jmethodID  constructionID;
    jmethodID  setCbDataDescribeID;
    jmethodID  setSzColumnNameID;
    jmethodID  setwColumnWidthID;
    jmethodID  setnFormatID;
};

//游戏房间信息本地结构体
struct CMD_GR_ServerInfoNativeJNI{
    jclass     serverInfo_cls;
    jmethodID  constructionID;
    jmethodID  setwKindID;
    jmethodID  setwTableCountID;
    jmethodID  setwChairCountID;
    jmethodID  setDwVideoAddrID;
    jmethodID  setwGameGenreID;
    jmethodID  setCbHideUserInfoID;
};

//用户信息本地结构体
struct TagUserDataNativeJNI{
    jclass     userData_cls;
    jmethodID  constructionID;
    jmethodID  setwFaceID;
    jmethodID  setDwCustomFaceVerID;
    jmethodID  setDwUserID;
    jmethodID  setDwGroupID;
    jmethodID  setDwGameID;
    jmethodID  setDwUserRightID;
    jmethodID  setlLovelinessID;
    jmethodID  setDwMasterRightID;
    jmethodID  setSzNameID;
    jmethodID  setSzPasswordID;
    jmethodID  setSzPhoneID;
    jmethodID  setCbGenderID;
    jmethodID  setCbMemberOrderID;
    jmethodID  setCbMasterOrderID;
    jmethodID  setlInsureScoreID;
    jmethodID  setlGameGoldID;
    jmethodID  setlScoreID;
    jmethodID  setlWinCountID;
    jmethodID  setlLostCountID;
    jmethodID  setlDrawCountID;
    jmethodID  setlFleeCountID;
    jmethodID  setlExperienceID;
    jmethodID  setwMatchRankID;
    jmethodID  setlUsableScoreID;
    jmethodID  setwTableID;
    jmethodID  setwChairID;
    jmethodID  setCbUserStatusID;
    jmethodID  setCbIsOutID;
};

//用户状态本地结构体
struct CMD_GR_UserStatusNativeJNI{
    jclass     userStatus_cls;
    jmethodID  constructionID;
    jmethodID  setDwUserID;
    jmethodID  setwTableID;
    jmethodID  setwChairID;
    jmethodID  setCbUserStatusID;
};

//桌子状态数组本地结构体
struct CMD_GR_TableInfoNativeJNI{
    jclass     tableInfo_cls;
    jmethodID  constructionID;
    jmethodID  setwTableCountID;
    jmethodID  setpTableStatusID;
};

//桌子状态本地结构体
struct TagTableStatusNativeJNI{
    jclass     tagTableStatus_cls;
    jmethodID  constructionID;
    jmethodID  setbTableLockID;
    jmethodID  setbPlayStatusID;
};

//桌子状态信息本地结构体
struct CMD_GR_TableStatusNativeJNI{
    jclass     tableStatus_cls;
    jmethodID  constructionID;
    jmethodID  setwTableID;
    jmethodID  setbTableLockID;
    jmethodID  setbPlayStatusID;
};

void initColumnInfoNativeJNI(JNIEnv* env,const char* className);
void initTagColumnItemNativeJNI(JNIEnv* env,const char* className);
void initServerInfoNativeJNI(JNIEnv* env,const char* className);
void initTagUserDataNativeJNI(JNIEnv* env,const char* className);
void initUserStatusNativeJNI(JNIEnv* env,const char* className);
void initTableInfoNativeJNI(JNIEnv* env,const char* className);
void initTagTableStatusNativeJNI(JNIEnv* env,const char* className);
void initTableStatusNativeJNI(JNIEnv* env,const char* className);

#endif //CHATFRAME_ROOMJNI_HELP_H
