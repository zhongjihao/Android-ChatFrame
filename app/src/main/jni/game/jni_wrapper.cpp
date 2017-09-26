/*************************************************************************
    > File Name: jni_wrapper.cpp
    > Author: zhongjihao
    > Mail: zhongjihao100@163.com 
    > Created Time: Mon 03 Apr 2017 11:03:49 AM CST
 *************************************************************************/

#define LOG_TAG "Game-Jni"

#include <jni.h>
#include <vector>
#include "./plazaviewItem/include/CLogonPlaza.h"
#include "./plazaviewItem/include/PlazaJni_help.h"
#include "./roomviewItem/include/CRoomViewItem.h"
#include "./roomviewItem/include/RoomJni_help.h"
#include "../log.h"


static JavaVM* g_vm = NULL;
static std::vector<CRoomViewItem*> g_pRoom;
static int g_RoomIndex = -1;

//回调java接口OcxGameCallback结构体
static struct OcxCallbackImplNativeJNI{
	jclass    ocx_callback_cls;
	jobject   ocx_callback_obj;
	jmethodID ocx_callback_mid;
}g_ocxCallbackNativeJNI;

//定义java中类全名
static const char* classPathName = "com/android/chatframe/game/GameNativeInterface";

static void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess* logonSuccess, const char* acc, const char* pwd , const char* underWrite, const char* groupName, const char* stationPage);
static void OnSocketMainLogonFail(int errorCode, const char* errorDescribe);
static void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess* registerSuccess);
static void OnSocketMainRegisterFail(const char* errorDescribe);
static void OnSocketMainUnRegister(int retCode, const char* retDescribe);
static void OnSocketMainServerListGameType(tagGameType* pGametype,int wItemCount);
static void OnSocketMainServerListGameKind(tagGameKind* pGamekind,int wItemCount);
static void OnSocketMainServerListGameStation(tagGameStation* pGamestation,int wItemCount);
static void OnSocketMainServerListGameServer(tagGameServer* pGameserver,int wItemCount);
static void OnSocketMainLoginFinish();
static void OnSocketRoomLogonFail(int errorCode, const char* errorDescribe);
static void OnSocketRoomLogonFinish(CMD_GR_ColumnInfo* pColumnInfo,CMD_GR_ServerInfo* pServerInfo,int online);
static void OnSocketRoomSubUserCome(tagUserData* pUserData,const char* pUnderWrite,const char* pGroupName);
static void OnSocketRoomSubUserStatus(CMD_GR_UserStatus* pUserStatus);
static void OnSocketRoomTableInfo(CMD_GR_TableInfo* pTableInfo);
static void OnSocketRoomTableStatus(CMD_GR_TableStatus* pTableStatus);


extern struct CMD_GP_LogonSuccessNativeJNI g_LogonSuccessNativeJNI;
extern struct CMD_GP_RegisterSuccessNativeJNI g_RegisterSuccessNativeJNI;
extern struct tagGameTypeNativeJNI g_GameTypeNativeJNI;
extern struct tagGameKindNativeJNI g_GameKindNativeJNI;
extern struct tagGameStationNativeJNI g_GameStationNativeJNI;
extern struct tagGameServerNativeJNI g_GameServerNativeJNI;
extern struct CMD_GR_ColumnInfoNativeJNI g_ColumnInfoNativeJNI;
extern struct TagColumnItemNativeJNI g_TagColumnItemNativeJNI;
extern struct CMD_GR_ServerInfoNativeJNI g_ServerInfoNativeJNI;
extern struct TagUserDataNativeJNI g_TagUserDataNativeJNI;
extern struct CMD_GR_UserStatusNativeJNI g_UserStatusNativeJNI;
extern struct CMD_GR_TableStatusNativeJNI g_TableStatusNativeJNI;
extern struct CMD_GR_TableInfoNativeJNI g_TableInfoNativeJNI;
extern struct TagTableStatusNativeJNI g_TagTableStatusNativeJNI;

extern void initLogonSuccessNativeJNI(JNIEnv* env,const char* className);
extern void initRegisterSuccessNativeJNI(JNIEnv* env,const char* className);
extern void initGameTypeNativeJNI(JNIEnv* env,const char* className);
extern void initGameKindNativeJNI(JNIEnv* env,const char* className);
extern void initGameStationNativeJNI(JNIEnv* env,const char* className);
extern void initGameServerNativeJNI(JNIEnv* env,const char* className);
extern void initColumnInfoNativeJNI(JNIEnv* env,const char* className);
extern void initTagColumnItemNativeJNI(JNIEnv* env,const char* className);
extern void initServerInfoNativeJNI(JNIEnv* env,const char* className);
extern void initTagUserDataNativeJNI(JNIEnv* env,const char* className);

jboolean JNI_OnConnectGamePlaza(JNIEnv *env,jclass jcls __unused,jstring jserIp,jint port)
{
	LOGD("%s: =====GameClient==========",__FUNCTION__);
	const char* serIp = env ->GetStringUTFChars(jserIp, NULL);
	LOGD("%s: ===GameClient===java pass serIP: %s",__FUNCTION__,serIp);
	bool isConnect = CLogonPlaza::getInstance()->ConnectPlazaServer(serIp,port);
	env ->ReleaseStringUTFChars(jserIp, serIp);

	return (jboolean)isConnect;
}

void JNI_OnLoginGamePlaza(JNIEnv *env,jclass jcls __unused,jstring jaccount,jstring jpwd,jint userId)
{
	const char* acc = env ->GetStringUTFChars(jaccount, NULL);
	const char* pwd = env ->GetStringUTFChars(jpwd, NULL);
	LOGD("%s: ===GameClient===java pass acc: %s, pwd: %s",__FUNCTION__,acc,pwd);
	CLogonPlaza::getInstance()->LogonPlaza(acc,pwd,(int)userId);
	env ->ReleaseStringUTFChars(jaccount, acc);
	env ->ReleaseStringUTFChars(jpwd, pwd);
}

void JNI_OnRegisterGamePlaza(JNIEnv *env,jclass jcls __unused,jstring jaccount,jstring jpwd,jstring jphone)
{
	const char* acc = env ->GetStringUTFChars(jaccount, NULL);
	const char* pwd = env ->GetStringUTFChars(jpwd, NULL);
	const char* phone = env ->GetStringUTFChars(jphone, NULL);
	LOGD("%s: ===GameClient===java pass acc: %s, pwd: %s, phone: %s",__FUNCTION__,acc,pwd,phone);
	CLogonPlaza::getInstance()->RegisterAccount(acc,pwd,phone);
	env ->ReleaseStringUTFChars(jaccount, acc);
	env ->ReleaseStringUTFChars(jpwd, pwd);
	env ->ReleaseStringUTFChars(jphone, phone);
}

void JNI_OnRegisterOcxInterface(JNIEnv *env, jclass jcls, jobject jobj)
{
	jclass tmp = env ->GetObjectClass(jobj); //可以在子线程中使用
	//JNI 函数参数中 jobject 或者它的子类，其参数都是 local reference.
	//Local reference 只在这个 JNI函数中有效，JNI函数返回后，引用的对象就被释放，它的生命周期就结束了.
	//若要留着日后使用，则需根据这个 local reference 创建 global reference.
	//Global reference 不会被系统自动释放，它仅当被程序明确调用 DeleteGlobalRef 时才被回收.
	g_ocxCallbackNativeJNI.ocx_callback_cls = (jclass)env->NewGlobalRef(tmp);
	g_ocxCallbackNativeJNI.ocx_callback_obj = env ->NewGlobalRef(jobj);
	LOGD("%s: ======GameClient=====1=======",__FUNCTION__);
	initLogonSuccessNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GP_LogonSuccess");
	initRegisterSuccessNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GP_RegisterSuccess");
	initGameTypeNativeJNI(env,"com/android/chatframe/game/gametype/tagGameType");
	initGameKindNativeJNI(env,"com/android/chatframe/game/gametype/tagGameKind");
	initGameStationNativeJNI(env,"com/android/chatframe/game/gametype/tagGameStation");
	initGameServerNativeJNI(env,"com/android/chatframe/game/gametype/tagGameServer");
	initTagColumnItemNativeJNI(env,"com/android/chatframe/game/gametype/tagColumnItem");
	initColumnInfoNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GR_ColumnInfo");
	initServerInfoNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GR_ServerInfo");
	initTagUserDataNativeJNI(env,"com/android/chatframe/game/gametype/tagUserData");
	initUserStatusNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GR_UserStatus");
	initTableInfoNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GR_TableInfo");
	initTagTableStatusNativeJNI(env,"com/android/chatframe/game/gametype/tagTableStatus");
	initTableStatusNativeJNI(env,"com/android/chatframe/game/gametype/CMD_GR_TableStatus");
	LOGD("%s: ======GameClient=====2=======",__FUNCTION__);

	CLogonPlaza::getInstance()->OnSetLoginSuccessCallback(OnSocketMainLogonSuccess);
	CLogonPlaza::getInstance()->OnSetLoginFailCallback(OnSocketMainLogonFail);
	CLogonPlaza::getInstance()->OnSetRegisterSuccessCallback(OnSocketMainRegisterSuccess);
	CLogonPlaza::getInstance()->OnSetUnRegisterCallback(OnSocketMainUnRegister);
	CLogonPlaza::getInstance()->OnSetRegisterFailCallback(OnSocketMainRegisterFail);
	CLogonPlaza::getInstance()->OnSetGameTypeCallback(OnSocketMainServerListGameType);
	CLogonPlaza::getInstance()->OnSetGameKindCallback(OnSocketMainServerListGameKind);
	CLogonPlaza::getInstance()->OnSetGameStationCallback(OnSocketMainServerListGameStation);
	CLogonPlaza::getInstance()->OnSetGameServerCallback(OnSocketMainServerListGameServer);
	CLogonPlaza::getInstance()->OnSetLoginFinishCallback(OnSocketMainLoginFinish);
}

void JNI_OnUnRegisterGamePlaza(JNIEnv *env __unused,jclass jcls __unused,jint userId)
{
	LOGD("%s: ===GameClient===java pass uid: %d",__FUNCTION__,(int)userId);
	CLogonPlaza::getInstance()->UnRegisterAccount((int)userId);
}

jboolean JNI_OnConnectGameRoom(JNIEnv *env,jclass jcls __unused,jstring jserIp,jint jport,jint jindex)
{
	g_RoomIndex = jindex;
	const char* serIp = env ->GetStringUTFChars(jserIp, NULL);
	LOGD("%s: ===GameClient===java pass serIP: %s, port: %d, index: %d",__FUNCTION__,serIp,jport,jindex);
	if(g_pRoom[jindex] == NULL)
		g_pRoom[jindex] = new CRoomViewItem;
	g_pRoom[jindex]->OnSetLoginFailCallback(OnSocketRoomLogonFail);
	g_pRoom[jindex]->OnSetLoginFinishCallback(OnSocketRoomLogonFinish);
	g_pRoom[jindex]->OnSetUserComeCallback(OnSocketRoomSubUserCome);
	g_pRoom[jindex]->OnSetUserStatusCallback(OnSocketRoomSubUserStatus);
	g_pRoom[jindex]->OnSetTableStatusCallback(OnSocketRoomTableStatus);
	g_pRoom[jindex]->OnSetTableInfoCallback(OnSocketRoomTableInfo);
	bool isConnect = g_pRoom[jindex]->ConnectGameRoomServer(serIp,(int)jport);
	env ->ReleaseStringUTFChars(jserIp, serIp);

	return (jboolean)isConnect;
}

void JNI_OnLoginGameRoom(JNIEnv *env __unused,jclass jcls __unused)
{
	LOGD("%s: ===GameClient===java pass  g_RoomIndex: %d",__FUNCTION__,g_RoomIndex);
	tagClientSerial* pSerial = CLogonPlaza::getInstance()->GetClientSerial();
	CMD_GR_LogonByUserID* pLogonId = CLogonPlaza::getInstance()->GetLogonId();
	g_pRoom[g_RoomIndex]->LogonGameRoom(pLogonId,pSerial);
}

void JNI_OnSitDown(JNIEnv *env __unused,jclass jcls __unused,jshort jtableId,jshort jchairId)
{
	LOGD("%s: ===GameClient===java pass  tableId: %d, chairId: %d",__FUNCTION__,jtableId,jchairId);
	g_pRoom[g_RoomIndex]->SendSitDownRequest(jtableId,jchairId);
}

void JNI_OnStandUp(JNIEnv *env __unused,jclass jcls __unused,jshort jtableId,jshort jchairId)
{
	LOGD("%s: ===GameClient===java pass tableId: %d, chairId: %d",__FUNCTION__,jtableId,jchairId);
	g_pRoom[g_RoomIndex]->SendStandUpRequest(jtableId,jchairId);
}

void JNI_OnExitRoom(JNIEnv *env __unused,jclass jcls __unused,jshort jtableId,jshort jchairId)
{
	LOGD("%s: ===GameClient===java pass tableId: %d, chairId: %d",__FUNCTION__,jtableId,jchairId);
	g_pRoom[g_RoomIndex]->OnExitRoomServer(jtableId,jchairId);
}

void JNI_OnDeleteRoom(JNIEnv *env __unused,jclass jcls __unused)
{
	LOGD("%s: ===GameClient======",__FUNCTION__);
	g_pRoom[g_RoomIndex]->OnDeleteRoom();
	g_pRoom[g_RoomIndex] = NULL;
}

static JNIEnv* getJNIEnv(bool* needsDetach)
{
	JNIEnv* env = NULL;
	if(g_vm ->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
	{
		int status = g_vm ->AttachCurrentThread(&env, NULL);
		if(status < 0)
		{
			LOGE("%s: ====GameClient============failed to attach current thread=======",__FUNCTION__);
			return NULL;
		}
		*needsDetach = true;
	}
	return env;
}

static void OnSocketMainLogonSuccess(CMD_GP_LogonSuccess* logonSuccess, const char* acc, const char* pwd , const char* underWrite, const char* groupName, const char* stationPage)
{
	LOGD("%s: ===GameClient===dwUserID: %d, acc: %s, pwd: %s, underWrite: %s ,groupName: %s,stationPage: %s",__FUNCTION__,logonSuccess->dwUserID,acc,pwd,underWrite,groupName,stationPage);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainLogonSuccess", "(Lcom/android/chatframe/game/gametype/CMD_GP_LogonSuccess;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainLogonSuccess methodID========",__FUNCTION__);
				return;
			}

			//创建java对象CMD_GP_LogonSuccess
			jobject loginSuccess = env->NewObject(g_LogonSuccessNativeJNI.logonSuccess_cls,g_LogonSuccessNativeJNI.constructionID);
			//给java对象CMD_GP_LogonSuccess成员wFaceID赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setwFaceID,(jshort)logonSuccess->wFaceID);
			//给java对象CMD_GP_LogonSuccess成员cbGender赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setCbGenderID,(jbyte)logonSuccess->cbGender);
			//给java对象CMD_GP_LogonSuccess成员cbMember赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setCbMemberID,(jbyte)logonSuccess->cbMember);
			//给java对象CMD_GP_LogonSuccess成员dwUserID赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setDwUserID,(jint)logonSuccess->dwUserID);
			//给java对象CMD_GP_LogonSuccess成员dwGameID赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setDwGameID,(jint)logonSuccess->dwGameID);
			//给java对象CMD_GP_LogonSuccess成员dwExperience赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setDwExperienceID,(jint)logonSuccess->dwExperience);
			//给java对象CMD_GP_LogonSuccess成员lLoveliness赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setlLovelinessID,(jint)logonSuccess->lLoveliness);
			//给java对象CMD_GP_LogonSuccess成员lGoldCoin赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setlGoldCoinID,(jint)logonSuccess->lGoldCoin);
			//给java对象CMD_GP_LogonSuccess成员lScore赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setlScoreID,(jint)logonSuccess->lScore);
			//给java对象CMD_GP_LogonSuccess成员dwCustomFaceVer赋值
			env->CallVoidMethod(loginSuccess,g_LogonSuccessNativeJNI.setDwCustomFaceVerID,(jint)logonSuccess->dwCustomFaceVer);
			jstring jacc = env ->NewStringUTF(acc);
			jstring jpwd = env ->NewStringUTF(pwd);
			jstring junderWrite = env ->NewStringUTF(strlen(underWrite) >0 ? underWrite : "");
			jstring jgroupName = env ->NewStringUTF(strlen(groupName) > 0 ? groupName : "");
			jstring jstationPage = env ->NewStringUTF(strlen(stationPage) > 0 ? stationPage : "");

			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,loginSuccess,jacc,jpwd,junderWrite,jgroupName,jstationPage);
			env ->DeleteLocalRef(loginSuccess);

			env->DeleteLocalRef(jacc);
			env->DeleteLocalRef(jpwd);
			if(junderWrite != NULL)
				env->DeleteLocalRef(junderWrite);
			if(jgroupName != NULL)
				env->DeleteLocalRef(jgroupName);
			if(jstationPage != NULL)
				env->DeleteLocalRef(jstationPage);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainLogonFail(int errorCode, const char* errorDescribe)
{
	LOGD("%s: ===GameClient===errorCode: %d, errorDescribe: %s",__FUNCTION__,errorCode,errorDescribe);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainLogonFail", "(ILjava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainLogonFail methodID========",__FUNCTION__);
				return;
			}

			jstring jerrorDescribe = env ->NewStringUTF(errorDescribe);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,(jint)errorCode,jerrorDescribe);
			env->DeleteLocalRef(jerrorDescribe);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainRegisterSuccess(CMD_GP_RegisterSuccess* registerSuccess)
{
	LOGD("%s: ===GameClient===szAcccount: %s, szPassword: %s,   phone: %s",__FUNCTION__,registerSuccess->szAcccount,registerSuccess->szPassword,registerSuccess->szPhone);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{

			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainRegisterSuccess", "(Lcom/android/chatframe/game/gametype/CMD_GP_RegisterSuccess;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainRegisterSuccess methodID========",__FUNCTION__);
				return;
			}

			//创建java对象CMD_GP_LogonSuccess
			jobject jregisterSuccess = env->NewObject(g_RegisterSuccessNativeJNI.registerSuccess_cls,g_RegisterSuccessNativeJNI.constructionID);
			//给java对象CMD_GP_RegisterSuccess成员wFaceID赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setwFaceID,(jshort)registerSuccess->wFaceID);
			//给java对象CMD_GP_LogonSuccess成员cbGender赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setCbGenderID,(jbyte)registerSuccess->cbGender);
			//给java对象CMD_GP_LogonSuccess成员cbMember赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setCbMemberID,(jbyte)registerSuccess->cbMember);
			//给java对象CMD_GP_LogonSuccess成员dwUserID赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setDwUserID,(jint)registerSuccess->dwUserID);
			//给java对象CMD_GP_LogonSuccess成员dwGameID赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setDwGameID,(jint)registerSuccess->dwGameID);
			//给java对象CMD_GP_LogonSuccess成员dwExperience赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setDwExperienceID,(jint)registerSuccess->dwExperience);
			//给java对象CMD_GP_LogonSuccess成员lLoveliness赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setlLovelinessID,(jint)registerSuccess->lLoveliness);
			//给java对象CMD_GP_LogonSuccess成员lGoldCoin赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setlGoldCoinID,(jint)registerSuccess->lGoldCoin);
			//给java对象CMD_GP_LogonSuccess成员lScore赋值
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setlScoreID,(jint)registerSuccess->lScore);
			//给java对象CMD_GP_LogonSuccess成员szAcccount赋值
			jstring jacc = env ->NewStringUTF(registerSuccess->szAcccount);
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setSzAcccountID,jacc);
			//给java对象CMD_GP_LogonSuccess成员setSzPassword赋值
			jstring jpwd = env ->NewStringUTF(registerSuccess->szPassword);
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setSzPasswordID,jpwd);
			//给java对象CMD_GP_LogonSuccess成员setSzPhone赋值
			jstring jphone = env ->NewStringUTF(registerSuccess->szPhone);
			env->CallVoidMethod(jregisterSuccess,g_RegisterSuccessNativeJNI.setSzPhoneID,jphone);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jregisterSuccess);
			env ->DeleteLocalRef(jregisterSuccess);
			env->DeleteLocalRef(jacc);
			env->DeleteLocalRef(jpwd);
			env->DeleteLocalRef(jphone);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainRegisterFail(const char* errorDescribe)
{
	LOGD("%s: ===GameClient===errorDescribe: %s",__FUNCTION__,errorDescribe);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainRegisterFail", "(Ljava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainRegisterFail methodID========",__FUNCTION__);
				return;
			}

			jstring jerrorDescribe = env ->NewStringUTF(errorDescribe);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jerrorDescribe);
			env->DeleteLocalRef(jerrorDescribe);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainUnRegister(int retCode, const char* retDescribe)
{
	LOGD("%s: ===GameClient===retCode: %d, retDescribe: %s",__FUNCTION__,retCode,retDescribe);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainUnRegister", "(ILjava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainUnRegister methodID========",__FUNCTION__);
				return;
			}

			jstring jerrorDescribe = env ->NewStringUTF(retDescribe);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,(jint)retCode,jerrorDescribe);
			env->DeleteLocalRef(jerrorDescribe);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainLoginFinish()
{
	LOGD("%s: ===GameClient===",__FUNCTION__);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainLoginFinish", "()V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainLoginFinish methodID========",__FUNCTION__);
				return;
			}

			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainServerListGameType(tagGameType* pGametype,int wItemCount)
{
	LOGD("%s: ===GameClient===wItemCount: %d",__FUNCTION__,wItemCount);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainServerListGameType", "([Lcom/android/chatframe/game/gametype/tagGameType;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainServerListGameType methodID========",__FUNCTION__);
				return;
			}
			//创建一个元素个数为wItemCount的对象tagGameType数组
			jobjectArray resultArray = env->NewObjectArray(wItemCount,g_GameTypeNativeJNI.gameType_cls,NULL);
			for(int i = 0;i < wItemCount;i++)
			{
				//创建java对象tagGameType
				jobject jgameType = env->NewObject(g_GameTypeNativeJNI.gameType_cls,g_GameTypeNativeJNI.constructionID);
				//给java对象tagGameType成员wSortID赋值
				env->CallVoidMethod(jgameType,g_GameTypeNativeJNI.setwSortID,(jshort)((pGametype+i)->wSortID));
				//给java对象tagGameType成员wTypeID赋值
				env->CallVoidMethod(jgameType,g_GameTypeNativeJNI.setwTypeID,(jshort)((pGametype+i)->wTypeID));
				//给java对象tagGameType成员szTypeName赋值
				jstring jtypeName = env ->NewStringUTF((pGametype+i)->szTypeName);
				env->CallVoidMethod(jgameType,g_GameTypeNativeJNI.setSzTypeName,jtypeName);

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(resultArray, i,jgameType);
				env->DeleteLocalRef(jtypeName);
				env->DeleteLocalRef(jgameType);
			}
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,resultArray);
			//释放jobjectArray数组
			env->DeleteLocalRef(resultArray);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainServerListGameKind(tagGameKind* pGamekind,int wItemCount)
{
	LOGD("%s: ===GameClient===wItemCount: %d",__FUNCTION__,wItemCount);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainServerListGameKind", "([Lcom/android/chatframe/game/gametype/tagGameKind;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainServerListGameKind methodID========",__FUNCTION__);
				return;
			}
			//创建一个元素个数为wItemCount的对象tagGameKind数组
			jobjectArray resultArray = env->NewObjectArray(wItemCount,g_GameKindNativeJNI.gameKind_cls,NULL);
			for(int i = 0;i < wItemCount;i++)
			{
				//创建java对象tagGameKind
				jobject jgameKind = env->NewObject(g_GameKindNativeJNI.gameKind_cls,g_GameKindNativeJNI.constructionID);
				//给java对象tagGameKind成员wSortID赋值
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setwSortID,(jshort)((pGamekind+i)->wSortID));
				//给java对象tagGameKind成员wTypeID赋值
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setwTypeID,(jshort)((pGamekind+i)->wTypeID));
				//给java对象tagGameKind成员wKindID赋值
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setwKindID,(jshort)((pGamekind+i)->wKindID));
				//给java对象tagGameKind成员dwMaxVersion赋值
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setDwMaxVersionID,(jlong)((pGamekind+i)->dwMaxVersion));
				//给java对象tagGameKind成员dwOnLineCount赋值
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setDwOnLineCountID,(jlong)((pGamekind+i)->dwOnLineCount));
				//给java对象tagGameKind成员szKindName赋值
				jstring jkindName = env ->NewStringUTF((pGamekind+i)->szKindName);
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setSzKindNameID,jkindName);
				//给java对象tagGameKind成员szProcessName赋值
				jstring jprocessName = env ->NewStringUTF((pGamekind+i)->szProcessName);
				env->CallVoidMethod(jgameKind,g_GameKindNativeJNI.setSzProcessNameID,jprocessName);

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(resultArray, i,jgameKind);
				env->DeleteLocalRef(jkindName);
				env->DeleteLocalRef(jprocessName);
				env->DeleteLocalRef(jgameKind);
			}
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,resultArray);
			//释放jobjectArray数组
			env->DeleteLocalRef(resultArray);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainServerListGameStation(tagGameStation* pGamestation,int wItemCount)
{
	LOGD("%s: ===GameClient===wItemCount: %d",__FUNCTION__,wItemCount);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainServerListGameStation", "([Lcom/android/chatframe/game/gametype/tagGameStation;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainServerListGameStation methodID========",__FUNCTION__);
				return;
			}
			//创建一个元素个数为wItemCount的对象tagGameStation数组
			jobjectArray resultArray = env->NewObjectArray(wItemCount,g_GameStationNativeJNI.gameStation_cls,NULL);
			for(int i = 0;i < wItemCount;i++)
			{
				//创建java对象tagGameStation
				jobject jgameStation = env->NewObject(g_GameStationNativeJNI.gameStation_cls,g_GameStationNativeJNI.constructionID);
				//给java对象tagGameStation成员wSortID赋值
				env->CallVoidMethod(jgameStation,g_GameStationNativeJNI.setwSortID,(jshort)((pGamestation+i)->wSortID));
				//给java对象tagGameStation成员wKindID赋值
				env->CallVoidMethod(jgameStation,g_GameStationNativeJNI.setwKindID,(jshort)((pGamestation+i)->wKindID));
				//给java对象tagGameStation成员wJoinID赋值
				env->CallVoidMethod(jgameStation,g_GameStationNativeJNI.setwJoinID,(jshort)((pGamestation+i)->wJoinID));
				//给java对象tagGameStation成员wStationID赋值
				env->CallVoidMethod(jgameStation,g_GameStationNativeJNI.setwStationID,(jshort)((pGamestation+i)->wStationID));
				//给java对象tagGameStation成员szStationName赋值
				jstring jstationName = env ->NewStringUTF((pGamestation+i)->szStationName);
				env->CallVoidMethod(jgameStation,g_GameStationNativeJNI.setSzStationNameID,jstationName);

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(resultArray, i,jgameStation);
				env->DeleteLocalRef(jstationName);
				env->DeleteLocalRef(jgameStation);
			}
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,resultArray);
			//释放jobjectArray数组
			env->DeleteLocalRef(resultArray);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketMainServerListGameServer(tagGameServer* pGameserver,int wItemCount)
{
	LOGD("%s: ===GameClient===wItemCount: %d",__FUNCTION__,wItemCount);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainServerListGameServer", "([Lcom/android/chatframe/game/gametype/tagGameServer;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainServerListGameServer methodID========",__FUNCTION__);
				return;
			}
			for(int i=0;i<wItemCount;i++)
			{
				CRoomViewItem* item = new CRoomViewItem;
				g_pRoom.push_back(item);
			}
			//创建一个元素个数为wItemCount的对象tagGameServer数组
			jobjectArray resultArray = env->NewObjectArray(wItemCount,g_GameServerNativeJNI.gameServer_cls,NULL);
			for(int i = 0;i < wItemCount;i++)
			{
				//创建java对象tagGameServer
				jobject jgameServer = env->NewObject(g_GameServerNativeJNI.gameServer_cls,g_GameServerNativeJNI.constructionID);
				//给java对象tagGameServer成员wServerType赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwServerTypeID,(jshort)((pGameserver+i)->wServerType));
				//给java对象tagGameServer成员wSortID赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwSortID,(jshort)((pGameserver+i)->wSortID));
				//给java对象tagGameServer成员wKindID赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwKindID,(jshort)((pGameserver+i)->wKindID));
				//给java对象tagGameServer成员wServerID赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwServerID,(jshort)((pGameserver+i)->wServerID));
				//给java对象tagGameServer成员wStationID赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwStationID,(jshort)((pGameserver+i)->wStationID));
				//给java对象tagGameServer成员wServerPort赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setwServerPortID,(jshort)((pGameserver+i)->wServerPort));
				//给java对象tagGameServer成员dwServerAddr赋值
				char ip[32] = {0};
				struct in_addr addr;
				addr.s_addr = (pGameserver+i)->dwServerAddr;
				strcpy(ip,inet_ntoa(addr));
				jstring jip = env ->NewStringUTF(ip);
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setDwServerAddrID,jip);
				//给java对象tagGameServer成员dwOnLineCount赋值
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setDwOnLineCountID,(jlong)((pGameserver+i)->dwOnLineCount));
				//给java对象tagGameServer成员szServerName赋值
				jstring jserverName = env ->NewStringUTF((pGameserver+i)->szServerName);
				env->CallVoidMethod(jgameServer,g_GameServerNativeJNI.setSzServerNameID,jserverName);

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(resultArray, i,jgameServer);
				env->DeleteLocalRef(jserverName);
				env->DeleteLocalRef(jip);
				env->DeleteLocalRef(jgameServer);
			}
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,resultArray);
			//释放jobjectArray数组
			env->DeleteLocalRef(resultArray);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomLogonFail(int errorCode, const char* errorDescribe)
{
	LOGD("%s: ===GameClient===errorCode: %d, errorDescribe: %s",__FUNCTION__,errorCode,errorDescribe);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketRoomLogonFail", "(ILjava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketRoomLogonFail methodID========",__FUNCTION__);
				return;
			}

			jstring jerrorDescribe = env ->NewStringUTF(errorDescribe);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,(jint)errorCode,jerrorDescribe);
			env->DeleteLocalRef(jerrorDescribe);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomLogonFinish(CMD_GR_ColumnInfo* pColumnInfo,CMD_GR_ServerInfo* pServerInfo,int online)
{
	LOGD("%s: ===GameClient===online: %d",__FUNCTION__,online);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketRoomLogonFinish", "(Lcom/android/chatframe/game/gametype/CMD_GR_ColumnInfo;Lcom/android/chatframe/game/gametype/CMD_GR_ServerInfo;I)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketRoomLogonFinish methodID========",__FUNCTION__);
				return;
			}
			//创建java对象CMD_GR_ColumnInfo
			jobject jColumnInfo = env->NewObject(g_ColumnInfoNativeJNI.columnInfo_cls,g_ColumnInfoNativeJNI.constructionID);
			//给java对象CMD_GR_ColumnInfo成员wColumnCount赋值
			env->CallVoidMethod(jColumnInfo,g_ColumnInfoNativeJNI.setwColumnCountID,(jshort)pColumnInfo->wColumnCount);

			//创建一个元素个数为wColumnCount的对象tagColumnItem数组
			jobjectArray jtagColumnItemArray = env->NewObjectArray(pColumnInfo->wColumnCount,g_TagColumnItemNativeJNI.tagColumnItem_cls,NULL);
			for(int i = 0;i < pColumnInfo->wColumnCount;i++)
			{
				//创建java对象tagColumnItem
				jobject jtagColumnItem = env->NewObject(g_TagColumnItemNativeJNI.tagColumnItem_cls,g_TagColumnItemNativeJNI.constructionID);
				//给java对象tagColumnItem成员cbDataDescribe赋值
				env->CallVoidMethod(jtagColumnItem,g_TagColumnItemNativeJNI.setCbDataDescribeID,(jchar)((pColumnInfo->pColumnItem+i)->cbDataDescribe));
				//给java对象tagColumnItem成员szColumnName赋值
				jstring jcolumnName = env ->NewStringUTF((pColumnInfo->pColumnItem+i)->szColumnName);
				env->CallVoidMethod(jtagColumnItem,g_TagColumnItemNativeJNI.setSzColumnNameID,jcolumnName);
				//给java对象tagColumnItem成员wColumnWidth赋值
				env->CallVoidMethod(jtagColumnItem,g_TagColumnItemNativeJNI.setwColumnWidthID,(jshort)((pColumnInfo->pColumnItem+i)->wColumnWidth));
				//给java对象tagColumnItem成员nFormat赋值
				env->CallVoidMethod(jtagColumnItem,g_TagColumnItemNativeJNI.setnFormatID,(jshort)((pColumnInfo->pColumnItem+i)->nFormat));

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(jtagColumnItemArray, i,jtagColumnItem);
				env->DeleteLocalRef(jcolumnName);
				env->DeleteLocalRef(jtagColumnItem);
			}
			//给java对象CMD_GR_ColumnInfo成员pColumnItem赋值
			env->CallVoidMethod(jColumnInfo,g_ColumnInfoNativeJNI.setpColumnItemID,jtagColumnItemArray);

			//创建java对象CMD_GR_ServerInfo
			jobject jServerInfo = env->NewObject(g_ServerInfoNativeJNI.serverInfo_cls,g_ServerInfoNativeJNI.constructionID);
			//给java对象CMD_GR_ServerInfo成员wKindID赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setwKindID,(jshort)pServerInfo->wKindID);
			//给java对象CMD_GR_ServerInfo成员wTableCount赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setwTableCountID,(jshort)pServerInfo->wTableCount);
			//给java对象CMD_GR_ServerInfo成员wChairCount赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setwChairCountID,(jshort)pServerInfo->wChairCount);
			//给java对象CMD_GR_ServerInfo成员dwVideoAddr赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setDwVideoAddrID,(jint)pServerInfo->dwVideoAddr);
			//给java对象CMD_GR_ServerInfo成员wGameGenre赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setwGameGenreID,(jshort)pServerInfo->wGameGenre);
			//给java对象CMD_GR_ServerInfo成员cbHideUserInfo赋值
			env->CallVoidMethod(jServerInfo,g_ServerInfoNativeJNI.setCbHideUserInfoID,(jchar)pServerInfo->cbHideUserInfo);

			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jColumnInfo,jServerInfo,(jint)online);
			//释放jobjectArray数组
			env->DeleteLocalRef(jtagColumnItemArray);
			env->DeleteLocalRef(jColumnInfo);
			env->DeleteLocalRef(jServerInfo);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomSubUserCome(tagUserData* pUserData,const char* pUnderWrite,const char* pGroupName)
{
	LOGD("%s: =====GameClient===pUnderWrite: %s,   pGroupName: %s",__FUNCTION__,pUnderWrite,pGroupName);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketRoomSubUserCome", "(Lcom/android/chatframe/game/gametype/tagUserData;Ljava/lang/String;Ljava/lang/String;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketRoomSubUserCome methodID========",__FUNCTION__);
				return;
			}

			//创建java对象tagUserData
			jobject jUserData = env->NewObject(g_TagUserDataNativeJNI.userData_cls,g_TagUserDataNativeJNI.constructionID);
			//给java对象tagUserData成员wFaceID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setwFaceID,(jshort)pUserData->wFaceID);
			//给java对象tagUserData成员dwCustomFaceVer赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwCustomFaceVerID,(jlong)pUserData->dwCustomFaceVer);
			//给java对象tagUserData成员dwUserID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwUserID,(jlong)pUserData->dwUserID);
			//给java对象tagUserData成员dwGroupID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwGroupID,(jlong)pUserData->dwGroupID);
			//给java对象tagUserData成员dwGameID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwGameID,(jlong)pUserData->dwGameID);
			//给java对象tagUserData成员dwUserRight赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwUserRightID,(jlong)pUserData->dwUserRight);
			//给java对象tagUserData成员lLoveliness赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlLovelinessID,(jlong)pUserData->lLoveliness);
			//给java对象tagUserData成员dwMasterRight赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setDwMasterRightID,(jlong)pUserData->dwMasterRight);
			//给java对象tagUserData成员szName赋值
			jstring jName = env ->NewStringUTF(pUserData->szName);
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setSzNameID,jName);
			//给java对象tagUserData成员szPassword赋值
			jstring jpwd = env ->NewStringUTF(pUserData->szPassword);
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setSzPasswordID,jpwd);
			//给java对象tagUserData成员szPhone赋值
			jstring jphone = env ->NewStringUTF(pUserData->szPhone);
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setSzPhoneID,jphone);

			//给java对象tagUserData成员cbGender赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setCbGenderID,(jbyte)pUserData->cbGender);
			//给java对象tagUserData成员cbMemberOrder赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setCbMemberOrderID,(jbyte)pUserData->cbMemberOrder);
			//给java对象tagUserData成员cbMasterOrder赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setCbMasterOrderID,(jbyte)pUserData->cbMasterOrder);
			//给java对象tagUserData成员lInsureScore赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlInsureScoreID,(jlong)pUserData->lInsureScore);
			//给java对象tagUserData成员lGameGold赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlGameGoldID,(jlong)pUserData->lGameGold);
			//给java对象tagUserData成员lScore赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlScoreID,(jlong)pUserData->lScore);
			//给java对象tagUserData成员lWinCount赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlWinCountID,(jlong)pUserData->lWinCount);
			//给java对象tagUserData成员lLostCount赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlLostCountID,(jlong)pUserData->lLostCount);
			//给java对象tagUserData成员lDrawCount赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlDrawCountID,(jlong)pUserData->lDrawCount);
			//给java对象tagUserData成员lFleeCount赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlFleeCountID,(jlong)pUserData->lFleeCount);
			//给java对象tagUserData成员lExperience赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlExperienceID,(jlong)pUserData->lExperience);
			//给java对象tagUserData成员wMatchRank赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setwMatchRankID,(jshort)pUserData->wMatchRank);
			//给java对象tagUserData成员lUsableScore赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setlUsableScoreID,(jint)pUserData->lUsableScore);
			//给java对象tagUserData成员wTableID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setwTableID,(jshort)pUserData->wTableID);
			//给java对象tagUserData成员wChairID赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setwChairID,(jshort)pUserData->wChairID);
			//给java对象tagUserData成员cbUserStatus赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setCbUserStatusID,(jbyte)pUserData->cbUserStatus);
			//给java对象tagUserData成员cbIsOut赋值
			env->CallVoidMethod(jUserData,g_TagUserDataNativeJNI.setCbIsOutID,(jboolean)pUserData->cbIsOut);
			jstring junderWrite = env ->NewStringUTF(pUnderWrite);
			jstring jgroupName = env ->NewStringUTF(pGroupName);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jUserData,junderWrite,jgroupName);
			//释放
			env->DeleteLocalRef(jName);
			env->DeleteLocalRef(jpwd);
			env->DeleteLocalRef(jphone);
			env->DeleteLocalRef(junderWrite);
			env->DeleteLocalRef(jgroupName);
			env->DeleteLocalRef(jUserData);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomSubUserStatus(CMD_GR_UserStatus* pUserStatus)
{
	LOGD("%s: =====GameClient===dwUserID: %ld,   wTableID: %d  wChairID: %d   cbUserStatus: %d",__FUNCTION__,
		              pUserStatus->dwUserID,pUserStatus->wTableID,pUserStatus->wChairID,pUserStatus->cbUserStatus);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketRoomSubUserStatus", "(Lcom/android/chatframe/game/gametype/CMD_GR_UserStatus;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketRoomSubUserStatus methodID========",__FUNCTION__);
				return;
			}
			//创建java对象CMD_GR_UserStatus
			jobject jUserStatus = env->NewObject(g_UserStatusNativeJNI.userStatus_cls,g_UserStatusNativeJNI.constructionID);
			//给java对象CMD_GR_UserStatus成员dwUserID赋值
			env->CallVoidMethod(jUserStatus,g_UserStatusNativeJNI.setDwUserID,(jlong)pUserStatus->dwUserID);
			//给java对象CMD_GR_UserStatus成员wTableID赋值
			env->CallVoidMethod(jUserStatus,g_UserStatusNativeJNI.setwTableID,(jshort)pUserStatus->wTableID);
			//给java对象CMD_GR_UserStatus成员wChairID赋值
			env->CallVoidMethod(jUserStatus,g_UserStatusNativeJNI.setwChairID,(jshort)pUserStatus->wChairID);
			//给java对象CMD_GR_UserStatus成员cbUserStatus赋值
			env->CallVoidMethod(jUserStatus,g_UserStatusNativeJNI.setCbUserStatusID,(jbyte)pUserStatus->cbUserStatus);
			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jUserStatus);
			//释放
			env->DeleteLocalRef(jUserStatus);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomTableStatus(CMD_GR_TableStatus* pTableStatus)
{
	LOGD("%s: =====GameClient===wTableID: %d,   bTableLock: %d  bPlayStatus: %d",__FUNCTION__,
		 pTableStatus->wTableID,pTableStatus->bTableLock,pTableStatus->bPlayStatus);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainTableStatus", "(Lcom/android/chatframe/game/gametype/CMD_GR_TableStatus;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainTableStatus methodID========",__FUNCTION__);
				return;
			}

			//创建java对象CMD_GR_TableStatus
			jobject jTableStatus = env->NewObject(g_TableStatusNativeJNI.tableStatus_cls,g_TableStatusNativeJNI.constructionID);
			//给java对象CMD_GR_TableStatus成员wTableID赋值
			env->CallVoidMethod(jTableStatus,g_TableStatusNativeJNI.setwTableID,(jshort)pTableStatus->wTableID);
			//给java对象CMD_GR_TableStatus成员bTableLock赋值
			env->CallVoidMethod(jTableStatus,g_TableStatusNativeJNI.setbTableLockID,(jbyte)pTableStatus->bTableLock);
			//给java对象CMD_GR_TableStatus成员bPlayStatus赋值
			env->CallVoidMethod(jTableStatus,g_TableStatusNativeJNI.setbPlayStatusID,(jbyte)pTableStatus->bPlayStatus);

			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jTableStatus);
			//释放
			env->DeleteLocalRef(jTableStatus);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

static void OnSocketRoomTableInfo(CMD_GR_TableInfo* pTableInfo)
{
	LOGD("%s: ===GameClient===wTableCount: %d",__FUNCTION__,pTableInfo->wTableCount);
	bool needsDetach = false;
	JNIEnv * env = getJNIEnv(&needsDetach);
	if(env != NULL)
	{
		if(g_ocxCallbackNativeJNI.ocx_callback_cls != NULL && g_ocxCallbackNativeJNI.ocx_callback_obj != NULL)
		{
			g_ocxCallbackNativeJNI.ocx_callback_mid = env ->GetMethodID(g_ocxCallbackNativeJNI.ocx_callback_cls, "OnSocketMainTableInfo", "(Lcom/android/chatframe/game/gametype/CMD_GR_TableInfo;)V");
			if(g_ocxCallbackNativeJNI.ocx_callback_mid == NULL){
				LOGE("%s: ===GameClient=====not get java OnSocketMainTableInfo methodID========",__FUNCTION__);
				return;
			}
			//创建java对象CMD_GR_TableInfo
			jobject jTableInfo = env->NewObject(g_TableInfoNativeJNI.tableInfo_cls,g_TableInfoNativeJNI.constructionID);
			//给java对象CMD_GR_TableInfo成员wTableCount赋值
			env->CallVoidMethod(jTableInfo,g_TableInfoNativeJNI.setwTableCountID,(jshort)pTableInfo->wTableCount);

			//创建一个元素个数为wTableCount的对象tagTableStatus数组
			jobjectArray jtagTableStatusArray = env->NewObjectArray(pTableInfo->wTableCount,g_TagTableStatusNativeJNI.tagTableStatus_cls,NULL);
			for(int i = 0;i < pTableInfo->wTableCount;i++)
			{
				//创建java对象tagTableStatus
				jobject jtagTableStatus = env->NewObject(g_TagTableStatusNativeJNI.tagTableStatus_cls,g_TagTableStatusNativeJNI.constructionID);
				//给java对象tagTableStatus成员bTableLock赋值
				env->CallVoidMethod(jtagTableStatus,g_TagTableStatusNativeJNI.setbTableLockID,(jbyte)((pTableInfo->pTableStatus+i)->bTableLock));
				//给java对象tagTableStatus成员bPlayStatus赋值
				env->CallVoidMethod(jtagTableStatus,g_TagTableStatusNativeJNI.setbPlayStatusID,(jbyte)((pTableInfo->pTableStatus+i)->bPlayStatus));

				//给数组中的相应元素赋值
				env->SetObjectArrayElement(jtagTableStatusArray, i,jtagTableStatus);
				env->DeleteLocalRef(jtagTableStatus);
			}
			//给java对象CMD_GR_TableInfo成员pTableStatus赋值
			env->CallVoidMethod(jTableInfo,g_TableInfoNativeJNI.setpTableStatusID,jtagTableStatusArray);

			env ->CallVoidMethod(g_ocxCallbackNativeJNI.ocx_callback_obj, g_ocxCallbackNativeJNI.ocx_callback_mid,jTableInfo);
			//释放jobjectArray数组
			env->DeleteLocalRef(jtagTableStatusArray);
			env->DeleteLocalRef(jTableInfo);
		}
	}
	if(needsDetach)
	{
		g_vm ->DetachCurrentThread();
	}
	return;
}

//JAVA函数和C++函数映射关系表
static JNINativeMethod gMethods[] = {
	{ "connectGamePlaza","(Ljava/lang/String;I)Z",(void*)JNI_OnConnectGamePlaza},
	{ "loginGamePlaza","(Ljava/lang/String;Ljava/lang/String;I)V",(void*)JNI_OnLoginGamePlaza},
	{ "registerGamePlaza","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",(void*)JNI_OnRegisterGamePlaza},
	{ "unregisterGameAccount","(I)V",(void*)JNI_OnUnRegisterGamePlaza},
	{ "registerOcxInterface","(Lcom/android/chatframe/game/OcxGameCallback;)V",(void*)JNI_OnRegisterOcxInterface},
	{ "connectGameRoom","(Ljava/lang/String;II)Z",(void*)JNI_OnConnectGameRoom},
	{ "loginGameRoom","()V",(void*)JNI_OnLoginGameRoom},
	{ "sitDownReq","(SS)V",(void*)JNI_OnSitDown},
	{ "standUpReq","(SS)V",(void*)JNI_OnStandUp},
	{ "exitRoomReq","(SS)V",(void*)JNI_OnExitRoom},
	{ "deleteRoom","()V",(void*)JNI_OnDeleteRoom},
};

typedef union{
	JNIEnv* env;
	void* venv;
}UnionJNIEnvToVoid;

static jint registerNativeMethods(JNIEnv* env,const char* className,JNINativeMethod* gMethods,int numMethods)
{
	jclass clazz;
	clazz = env ->FindClass(className);

	if(clazz == NULL){
		return JNI_FALSE;
	}

	if(env->RegisterNatives(clazz,gMethods,numMethods) < 0){
		env ->DeleteLocalRef(clazz);
		return JNI_FALSE;
	}

	env ->DeleteLocalRef(clazz);
	return JNI_TRUE;
}


static jint registerNatives(JNIEnv* env,const char* classPathName,JNINativeMethod *gMethods,int methodsNum)
{
	if(!registerNativeMethods(env,classPathName,gMethods,methodsNum)){
		return JNI_FALSE;
	}
	return JNI_TRUE;
}

//JNI库加载时自动调用该函数
jint JNI_OnLoad(JavaVM* vm, void* reserved __unused)
{
	g_vm = vm;
	UnionJNIEnvToVoid uenv;
	JNIEnv* env = NULL;

	LOGD("%s: ==GameClient========1===",__FUNCTION__);
	//获得JNI版本
	if(vm->GetEnv((void**)&uenv.venv,JNI_VERSION_1_4) != JNI_OK){
		return -1;
	}

	LOGD("%s: ===GameClient======2====",__FUNCTION__);
	env = uenv.env;
	//注册java函数
	if(registerNatives(env,classPathName,gMethods,sizeof(gMethods)/sizeof(gMethods[0])) != JNI_TRUE){
		return -1;
	}

	LOGD("%s: ===GameClient======3===",__FUNCTION__);
	return JNI_VERSION_1_4;
}

//JNI库卸载时自动调用该函数
void JNI_OnUnload(JavaVM* jvm, void* reserved)
{
	LOGD("%s: ==GameClient========1===",__FUNCTION__);
	JNIEnv *env = NULL;
	if (jvm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}
	LOGD("%s: ==GameClient========2===",__FUNCTION__);
	env ->DeleteGlobalRef(g_LogonSuccessNativeJNI.logonSuccess_cls);
	env ->DeleteGlobalRef(g_RegisterSuccessNativeJNI.registerSuccess_cls);
	env ->DeleteGlobalRef(g_GameTypeNativeJNI.gameType_cls);
	env ->DeleteGlobalRef(g_GameKindNativeJNI.gameKind_cls);
	env ->DeleteGlobalRef(g_GameStationNativeJNI.gameStation_cls);
	env ->DeleteGlobalRef(g_GameServerNativeJNI.gameServer_cls);
	env ->DeleteGlobalRef(g_ColumnInfoNativeJNI.columnInfo_cls);
	env ->DeleteGlobalRef(g_TagColumnItemNativeJNI.tagColumnItem_cls);
	env ->DeleteGlobalRef(g_ServerInfoNativeJNI.serverInfo_cls);
	env ->DeleteGlobalRef(g_TagUserDataNativeJNI.userData_cls);
	env ->DeleteGlobalRef(g_UserStatusNativeJNI.userStatus_cls);
	env ->DeleteGlobalRef(g_TableStatusNativeJNI.tableStatus_cls);
	env ->DeleteGlobalRef(g_TableInfoNativeJNI.tableInfo_cls);
	env ->DeleteGlobalRef(g_TagTableStatusNativeJNI.tagTableStatus_cls);
	env ->DeleteGlobalRef(g_ocxCallbackNativeJNI.ocx_callback_obj);
	env ->DeleteGlobalRef(g_ocxCallbackNativeJNI.ocx_callback_cls);
}

