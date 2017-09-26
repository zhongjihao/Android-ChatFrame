//
// Created by SPREADTRUM\jihao.zhong on 17-8-2.
//

#define LOG_TAG "Plaza-Jni-Help"

#include <stdio.h>
#include "../include/PlazaJni_help.h"
#include "../../../log.h"

struct CMD_GP_LogonSuccessNativeJNI g_LogonSuccessNativeJNI;
struct CMD_GP_RegisterSuccessNativeJNI g_RegisterSuccessNativeJNI;
struct tagGameTypeNativeJNI g_GameTypeNativeJNI;
struct tagGameKindNativeJNI g_GameKindNativeJNI;
struct tagGameStationNativeJNI g_GameStationNativeJNI;
struct tagGameServerNativeJNI g_GameServerNativeJNI;

void initLogonSuccessNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_LogonSuccessNativeJNI.logonSuccess_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_LogonSuccessNativeJNI.logonSuccess_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class CMD_GP_LogonSuccess by java CMD_GP_LogonSuccess======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GP_LogonSuccess的构造函数ID
    if(g_LogonSuccessNativeJNI.constructionID == NULL){
        g_LogonSuccessNativeJNI.constructionID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls, "<init>", "()V");
        if(g_LogonSuccessNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class CMD_GP_LogonSuccess constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setwFaceID方法ID
    if(g_LogonSuccessNativeJNI.setwFaceID == NULL){
        g_LogonSuccessNativeJNI.setwFaceID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setwFaceID","(S)V");
        if(g_LogonSuccessNativeJNI.setwFaceID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class CMD_GP_LogonSuccess setwFaceID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setCbGender方法ID
    if(g_LogonSuccessNativeJNI.setCbGenderID == NULL){
        g_LogonSuccessNativeJNI.setCbGenderID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setCbGender","(B)V");
        if(g_LogonSuccessNativeJNI.setCbGenderID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_LogonSuccess setCbGender======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setCbMember方法ID
    if(g_LogonSuccessNativeJNI.setCbMemberID == NULL){
        g_LogonSuccessNativeJNI.setCbMemberID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setCbMember","(B)V");
        if(g_LogonSuccessNativeJNI.setCbMemberID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_LogonSuccess setCbMember======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setDwUserID方法ID
    if(g_LogonSuccessNativeJNI.setDwUserID == NULL){
        g_LogonSuccessNativeJNI.setDwUserID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setDwUserID","(I)V");
        if(g_LogonSuccessNativeJNI.setDwUserID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_LogonSuccess setDwUserID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setDwGameID方法ID
    if(g_LogonSuccessNativeJNI.setDwGameID == NULL){
        g_LogonSuccessNativeJNI.setDwGameID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setDwGameID","(I)V");
        if(g_LogonSuccessNativeJNI.setDwGameID == NULL){
            LOGE("%s: =====GameClient==not get c++ class CMD_GP_LogonSuccess setDwGameID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setDwExperience方法ID
    if(g_LogonSuccessNativeJNI.setDwExperienceID == NULL){
        g_LogonSuccessNativeJNI.setDwExperienceID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setDwExperience","(I)V");
        if(g_LogonSuccessNativeJNI.setDwExperienceID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_LogonSuccess setDwExperience======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setlLoveliness方法ID
    if(g_LogonSuccessNativeJNI.setlLovelinessID == NULL){
        g_LogonSuccessNativeJNI.setlLovelinessID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setlLoveliness","(I)V");
        if(g_LogonSuccessNativeJNI.setlLovelinessID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_LogonSuccess setlLoveliness======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setlGoldCoin方法ID
    if(g_LogonSuccessNativeJNI.setlGoldCoinID == NULL){
        g_LogonSuccessNativeJNI.setlGoldCoinID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setlGoldCoin","(I)V");
        if(g_LogonSuccessNativeJNI.setlGoldCoinID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_LogonSuccess setlGoldCoin======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setlScore方法ID
    if(g_LogonSuccessNativeJNI.setlScoreID == NULL){
        g_LogonSuccessNativeJNI.setlScoreID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setlScore","(I)V");
        if(g_LogonSuccessNativeJNI.setlScoreID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_LogonSuccess setlScore======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_LogonSuccess中setDwCustomFaceVer方法ID
    if(g_LogonSuccessNativeJNI.setDwCustomFaceVerID == NULL){
        g_LogonSuccessNativeJNI.setDwCustomFaceVerID = env->GetMethodID(g_LogonSuccessNativeJNI.logonSuccess_cls,"setDwCustomFaceVer","(I)V");
        if(g_LogonSuccessNativeJNI.setDwCustomFaceVerID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_LogonSuccess setDwCustomFaceVer======",__FUNCTION__);
            return;
        }
    }

    return;
}

void initRegisterSuccessNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_RegisterSuccessNativeJNI.registerSuccess_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_RegisterSuccessNativeJNI.registerSuccess_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class CMD_GP_RegisterSuccess by java CMD_GP_RegisterSuccess======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GP_RegisterSuccess的构造函数ID
    if(g_RegisterSuccessNativeJNI.constructionID == NULL){
        g_RegisterSuccessNativeJNI.constructionID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls, "<init>", "()V");
        if(g_RegisterSuccessNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class CMD_GP_RegisterSuccess constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setwFaceID方法ID
    if(g_RegisterSuccessNativeJNI.setwFaceID == NULL){
        g_RegisterSuccessNativeJNI.setwFaceID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setwFaceID","(S)V");
        if(g_RegisterSuccessNativeJNI.setwFaceID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class CMD_GP_RegisterSuccess setwFaceID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setCbGender方法ID
    if(g_RegisterSuccessNativeJNI.setCbGenderID == NULL){
        g_RegisterSuccessNativeJNI.setCbGenderID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setCbGender","(B)V");
        if(g_RegisterSuccessNativeJNI.setCbGenderID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_RegisterSuccess setCbGender======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类MD_GP_RegisterSuccess中setCbMember方法ID
    if(g_RegisterSuccessNativeJNI.setCbMemberID == NULL){
        g_RegisterSuccessNativeJNI.setCbMemberID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setCbMember","(B)V");
        if(g_RegisterSuccessNativeJNI.setCbMemberID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_RegisterSuccess setCbMember======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类MD_GP_RegisterSuccess中setDwUserID方法ID
    if(g_RegisterSuccessNativeJNI.setDwUserID == NULL){
        g_RegisterSuccessNativeJNI.setDwUserID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setDwUserID","(I)V");
        if(g_RegisterSuccessNativeJNI.setDwUserID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_RegisterSuccess setDwUserID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类MD_GP_RegisterSuccess中setDwGameID方法ID
    if(g_RegisterSuccessNativeJNI.setDwGameID == NULL){
        g_RegisterSuccessNativeJNI.setDwGameID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setDwGameID","(I)V");
        if(g_RegisterSuccessNativeJNI.setDwGameID == NULL){
            LOGE("%s: =====GameClient==not get c++ class CMD_GP_RegisterSuccess setDwGameID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setDwExperience方法ID
    if(g_RegisterSuccessNativeJNI.setDwExperienceID == NULL){
        g_RegisterSuccessNativeJNI.setDwExperienceID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setDwExperience","(I)V");
        if(g_RegisterSuccessNativeJNI.setDwExperienceID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_RegisterSuccess setDwExperience======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setlLoveliness方法ID
    if(g_RegisterSuccessNativeJNI.setlLovelinessID == NULL){
        g_RegisterSuccessNativeJNI.setlLovelinessID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setlLoveliness","(I)V");
        if(g_RegisterSuccessNativeJNI.setlLovelinessID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_RegisterSuccess setlLoveliness======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setlGoldCoin方法ID
    if(g_RegisterSuccessNativeJNI.setlGoldCoinID == NULL){
        g_RegisterSuccessNativeJNI.setlGoldCoinID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setlGoldCoin","(I)V");
        if(g_RegisterSuccessNativeJNI.setlGoldCoinID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_RegisterSuccess setlGoldCoin======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setlScore方法ID
    if(g_RegisterSuccessNativeJNI.setlScoreID == NULL){
        g_RegisterSuccessNativeJNI.setlScoreID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setlScore","(I)V");
        if(g_RegisterSuccessNativeJNI.setlScoreID == NULL){
            LOGE("%s: ====GameClient===not get c++ class CMD_GP_RegisterSuccess setlScore======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setSzAcccount方法ID
    if(g_RegisterSuccessNativeJNI.setSzAcccountID == NULL){
        g_RegisterSuccessNativeJNI.setSzAcccountID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setSzAcccount","(Ljava/lang/String;)V");
        if(g_RegisterSuccessNativeJNI.setSzAcccountID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_RegisterSuccess setSzAcccount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setSzPassword方法ID
    if(g_RegisterSuccessNativeJNI.setSzPasswordID == NULL){
        g_RegisterSuccessNativeJNI.setSzPasswordID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setSzPassword","(Ljava/lang/String;)V");
        if(g_RegisterSuccessNativeJNI.setSzPasswordID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_RegisterSuccess setSzPassword======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GP_RegisterSuccess中setSzPhone方法ID
    if(g_RegisterSuccessNativeJNI.setSzPhoneID == NULL){
        g_RegisterSuccessNativeJNI.setSzPhoneID = env->GetMethodID(g_RegisterSuccessNativeJNI.registerSuccess_cls,"setSzPhone","(Ljava/lang/String;)V");
        if(g_RegisterSuccessNativeJNI.setSzPhoneID == NULL){
            LOGE("%s: ===GameClient====not get c++ class CMD_GP_RegisterSuccess setSzPhone======",__FUNCTION__);
            return;
        }
    }

    return;
}

void initGameTypeNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_GameTypeNativeJNI.gameType_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_GameTypeNativeJNI.gameType_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class tagGameType by java tagGameType======",__FUNCTION__);
        return;
    }
    //获取java类tagGameType的构造函数ID
    if(g_GameTypeNativeJNI.constructionID == NULL){
        g_GameTypeNativeJNI.constructionID = env->GetMethodID(g_GameTypeNativeJNI.gameType_cls, "<init>", "()V");
        if(g_GameTypeNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class tagGameType constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameType中setwSortID方法ID
    if(g_GameTypeNativeJNI.setwSortID == NULL){
        g_GameTypeNativeJNI.setwSortID = env->GetMethodID(g_GameTypeNativeJNI.gameType_cls,"setwSortID","(S)V");
        if(g_GameTypeNativeJNI.setwSortID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class tagGameType setwSortID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameType中setwTypeID方法ID
    if(g_GameTypeNativeJNI.setwTypeID == NULL){
        g_GameTypeNativeJNI.setwTypeID = env->GetMethodID(g_GameTypeNativeJNI.gameType_cls,"setwTypeID","(S)V");
        if(g_GameTypeNativeJNI.setwTypeID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameType setwTypeID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameType中setSzTypeName方法ID
    if(g_GameTypeNativeJNI.setSzTypeName == NULL){
        g_GameTypeNativeJNI.setSzTypeName = env->GetMethodID(g_GameTypeNativeJNI.gameType_cls,"setSzTypeName","(Ljava/lang/String;)V");
        if(g_GameTypeNativeJNI.setSzTypeName == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameType setSzTypeName======",__FUNCTION__);
            return;
        }
    }
}

void initGameKindNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_GameKindNativeJNI.gameKind_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_GameKindNativeJNI.gameKind_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class tagGameKind by java tagGameKind======",__FUNCTION__);
        return;
    }
    //获取java类tagGameKind的构造函数ID
    if(g_GameKindNativeJNI.constructionID == NULL){
        g_GameKindNativeJNI.constructionID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls, "<init>", "()V");
        if(g_GameKindNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class tagGameKind constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setwSortID方法ID
    if(g_GameKindNativeJNI.setwSortID == NULL){
        g_GameKindNativeJNI.setwSortID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setwSortID","(S)V");
        if(g_GameKindNativeJNI.setwSortID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class tagGameKind setwSortID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setwTypeID方法ID
    if(g_GameKindNativeJNI.setwTypeID == NULL){
        g_GameKindNativeJNI.setwTypeID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setwTypeID","(S)V");
        if(g_GameKindNativeJNI.setwTypeID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameKind setwTypeID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setwKindID方法ID
    if(g_GameKindNativeJNI.setwKindID == NULL){
        g_GameKindNativeJNI.setwKindID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setwKindID","(S)V");
        if(g_GameKindNativeJNI.setwKindID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameKind setwKindID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setDwMaxVersionID方法ID
    if(g_GameKindNativeJNI.setDwMaxVersionID == NULL){
        g_GameKindNativeJNI.setDwMaxVersionID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setDwMaxVersion","(J)V");
        if(g_GameKindNativeJNI.setDwMaxVersionID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameKind setDwMaxVersionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setDwOnLineCountID方法ID
    if(g_GameKindNativeJNI.setDwOnLineCountID == NULL){
        g_GameKindNativeJNI.setDwOnLineCountID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setDwOnLineCount","(J)V");
        if(g_GameKindNativeJNI.setDwOnLineCountID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameType setDwOnLineCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setSzKindNameID方法ID
    if(g_GameKindNativeJNI.setSzKindNameID == NULL){
        g_GameKindNativeJNI.setSzKindNameID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setSzKindName","(Ljava/lang/String;)V");
        if(g_GameKindNativeJNI.setSzKindNameID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameKind setSzKindNameID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameKind中setSzProcessNameID方法ID
    if(g_GameKindNativeJNI.setSzProcessNameID == NULL){
        g_GameKindNativeJNI.setSzProcessNameID = env->GetMethodID(g_GameKindNativeJNI.gameKind_cls,"setSzProcessName","(Ljava/lang/String;)V");
        if(g_GameKindNativeJNI.setSzProcessNameID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameKind setSzProcessNameID======",__FUNCTION__);
            return;
        }
    }
}

void initGameStationNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_GameStationNativeJNI.gameStation_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_GameStationNativeJNI.gameStation_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class tagGameStation by java tagGameStation======",__FUNCTION__);
        return;
    }
    //获取java类tagGameStation的构造函数ID
    if(g_GameStationNativeJNI.constructionID == NULL){
        g_GameStationNativeJNI.constructionID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls, "<init>", "()V");
        if(g_GameStationNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class tagGameStation constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameStation中setwSortID方法ID
    if(g_GameStationNativeJNI.setwSortID == NULL){
        g_GameStationNativeJNI.setwSortID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls,"setwSortID","(S)V");
        if(g_GameStationNativeJNI.setwSortID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class tagGameStation setwSortID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameStation中setwKindID方法ID
    if(g_GameStationNativeJNI.setwKindID == NULL){
        g_GameStationNativeJNI.setwKindID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls,"setwKindID","(S)V");
        if(g_GameStationNativeJNI.setwKindID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameStation setwKindID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameStation中setwJoinID方法ID
    if(g_GameStationNativeJNI.setwJoinID == NULL){
        g_GameStationNativeJNI.setwJoinID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls,"setwJoinID","(S)V");
        if(g_GameStationNativeJNI.setwJoinID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameStation setwJoinID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameStation中setwStationID方法ID
    if(g_GameStationNativeJNI.setwStationID == NULL){
        g_GameStationNativeJNI.setwStationID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls,"setwStationID","(S)V");
        if(g_GameStationNativeJNI.setwStationID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameStation setwStationID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameStation中setSzStationName方法ID
    if(g_GameStationNativeJNI.setSzStationNameID == NULL){
        g_GameStationNativeJNI.setSzStationNameID = env->GetMethodID(g_GameStationNativeJNI.gameStation_cls,"setSzStationName","(Ljava/lang/String;)V");
        if(g_GameStationNativeJNI.setSzStationNameID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameKind setSzStationNameID======",__FUNCTION__);
            return;
        }
    }
}

void initGameServerNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_GameServerNativeJNI.gameServer_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_GameServerNativeJNI.gameServer_cls == NULL){
        LOGE("%s: ===GameClient====not find c++ class tagGameServer by java tagGameServer======",__FUNCTION__);
        return;
    }
    //获取java类tagGameServer的构造函数ID
    if(g_GameServerNativeJNI.constructionID == NULL){
        g_GameServerNativeJNI.constructionID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls, "<init>", "()V");
        if(g_GameServerNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get c++ class tagGameServer constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameServer中setwServerTypeID方法ID
    if(g_GameServerNativeJNI.setwServerTypeID == NULL){
        g_GameServerNativeJNI.setwServerTypeID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwServerType","(S)V");
        if(g_GameServerNativeJNI.setwServerTypeID  == NULL){
            LOGE("%s: =====GameClient==not get c++ class tagGameServer setwServerType======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameServer中setwSortID方法ID
    if(g_GameServerNativeJNI.setwSortID == NULL){
        g_GameServerNativeJNI.setwSortID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwSortID","(S)V");
        if(g_GameServerNativeJNI.setwSortID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameServer setwSortID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameServer中setwKindID方法ID
    if(g_GameServerNativeJNI.setwKindID == NULL){
        g_GameServerNativeJNI.setwKindID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwKindID","(S)V");
        if(g_GameServerNativeJNI.setwKindID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameServer setwKindID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagGameServer中setwServerID方法ID
    if(g_GameServerNativeJNI.setwServerID == NULL){
        g_GameServerNativeJNI.setwServerID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwServerID","(S)V");
        if(g_GameServerNativeJNI.setwServerID == NULL){
            LOGE("%s: ====GameClient===not get c++ class tagGameServer setwServerID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameServer中setwStationID方法ID
    if(g_GameServerNativeJNI.setwStationID == NULL){
        g_GameServerNativeJNI.setwStationID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwStationID","(S)V");
        if(g_GameServerNativeJNI.setwStationID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameServer setwStationID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameServer中ssetwServerPortID方法ID
    if(g_GameServerNativeJNI.setwServerPortID == NULL){
        g_GameServerNativeJNI.setwServerPortID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setwServerPort","(S)V");
        if(g_GameServerNativeJNI.setwServerPortID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameServer setwServerPortID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameServer中setDwServerAddrID方法ID
    if(g_GameServerNativeJNI.setDwServerAddrID == NULL){
        g_GameServerNativeJNI.setDwServerAddrID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setDwServerAddr","(Ljava/lang/String;)V");
        if(g_GameServerNativeJNI.setDwServerAddrID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameServer setDwServerAddrID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameServer中setDwOnLineCountID方法ID
    if(g_GameServerNativeJNI.setDwOnLineCountID == NULL){
        g_GameServerNativeJNI.setDwOnLineCountID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setDwOnLineCount","(J)V");
        if(g_GameServerNativeJNI.setDwOnLineCountID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameServer setDwOnLineCountID======",__FUNCTION__);
            return;
        }
    }

    //获取JAVA类tagGameServer中setSzServerNameID方法ID
    if(g_GameServerNativeJNI.setSzServerNameID == NULL){
        g_GameServerNativeJNI.setSzServerNameID = env->GetMethodID(g_GameServerNativeJNI.gameServer_cls,"setSzServerName","(Ljava/lang/String;)V");
        if(g_GameServerNativeJNI.setSzServerNameID == NULL){
            LOGE("%s: ===GameClient====not get c++ class tagGameServer setSzServerNameID======",__FUNCTION__);
            return;
        }
    }
}
