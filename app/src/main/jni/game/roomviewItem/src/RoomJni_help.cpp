//
// Created by SPREADTRUM\jihao.zhong on 17-9-2.
//


#define LOG_TAG "Room-Jni-Help"

#include <stdio.h>
#include "../include/RoomJni_help.h"
#include "../../../log.h"

struct CMD_GR_ColumnInfoNativeJNI g_ColumnInfoNativeJNI;
struct TagColumnItemNativeJNI g_TagColumnItemNativeJNI;
struct CMD_GR_ServerInfoNativeJNI g_ServerInfoNativeJNI;
struct TagUserDataNativeJNI g_TagUserDataNativeJNI;
struct CMD_GR_UserStatusNativeJNI g_UserStatusNativeJNI;
struct CMD_GR_TableInfoNativeJNI g_TableInfoNativeJNI;
struct TagTableStatusNativeJNI g_TagTableStatusNativeJNI;
struct CMD_GR_TableStatusNativeJNI g_TableStatusNativeJNI;


void initColumnInfoNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_ColumnInfoNativeJNI.columnInfo_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_ColumnInfoNativeJNI.columnInfo_cls == NULL){
        LOGE("%s: ===GameClient====not find java class CMD_GR_ColumnInfo by java CMD_GR_ColumnInfo======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GR_ColumnInfo的构造函数ID
    if(g_ColumnInfoNativeJNI.constructionID == NULL){
        g_ColumnInfoNativeJNI.constructionID = env->GetMethodID(g_ColumnInfoNativeJNI.columnInfo_cls, "<init>", "()V");
        if(g_ColumnInfoNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class CMD_GR_ColumnInfo constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ColumnInfo中setwColumnCount方法ID
    if(g_ColumnInfoNativeJNI.setwColumnCountID == NULL){
        g_ColumnInfoNativeJNI.setwColumnCountID = env->GetMethodID(g_ColumnInfoNativeJNI.columnInfo_cls,"setwColumnCount","(S)V");
        if(g_ColumnInfoNativeJNI.setwColumnCountID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_ColumnInfo setwColumnCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ColumnInfo中setpColumnItem方法ID
    if(g_ColumnInfoNativeJNI.setpColumnItemID == NULL){
        g_ColumnInfoNativeJNI.setpColumnItemID = env->GetMethodID(g_ColumnInfoNativeJNI.columnInfo_cls,"setpColumnItem","([Lcom/android/chatframe/game/gametype/tagColumnItem;)V");
        if(g_ColumnInfoNativeJNI.setpColumnItemID == NULL){
            LOGE("%s: ====GameClient===not get java class CMD_GR_ColumnInfo setpColumnItem======",__FUNCTION__);
            return;
        }
    }
}

void initTagColumnItemNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_TagColumnItemNativeJNI.tagColumnItem_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_TagColumnItemNativeJNI.tagColumnItem_cls == NULL){
        LOGE("%s: ===GameClient====not find java class tagColumnItem by java tagColumnItem======",__FUNCTION__);
        return;
    }
    //获取java类tagColumnItem的构造函数ID
    if(g_TagColumnItemNativeJNI.constructionID == NULL){
        g_TagColumnItemNativeJNI.constructionID = env->GetMethodID(g_TagColumnItemNativeJNI.tagColumnItem_cls, "<init>", "()V");
        if(g_TagColumnItemNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class tagColumnItem constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagColumnItem中setCbDataDescribe方法ID
    if(g_TagColumnItemNativeJNI.setCbDataDescribeID == NULL){
        g_TagColumnItemNativeJNI.setCbDataDescribeID = env->GetMethodID(g_TagColumnItemNativeJNI.tagColumnItem_cls,"setCbDataDescribe","(C)V");
        if(g_TagColumnItemNativeJNI.setCbDataDescribeID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagColumnItem setCbDataDescribe======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagColumnItem中setSzColumnName方法ID
    if(g_TagColumnItemNativeJNI.setSzColumnNameID == NULL){
        g_TagColumnItemNativeJNI.setSzColumnNameID = env->GetMethodID(g_TagColumnItemNativeJNI.tagColumnItem_cls,"setSzColumnName","(Ljava/lang/String;)V");
        if(g_TagColumnItemNativeJNI.setSzColumnNameID == NULL){
            LOGE("%s: ====GameClient===not get java class tagColumnItem setSzColumnName======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagColumnItem中setwColumnWidth方法ID
    if(g_TagColumnItemNativeJNI.setwColumnWidthID == NULL){
        g_TagColumnItemNativeJNI.setwColumnWidthID = env->GetMethodID(g_TagColumnItemNativeJNI.tagColumnItem_cls,"setwColumnWidth","(S)V");
        if(g_TagColumnItemNativeJNI.setwColumnWidthID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagColumnItem setwColumnWidth======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagColumnItem中setnFormat方法ID
    if(g_TagColumnItemNativeJNI.setnFormatID == NULL){
        g_TagColumnItemNativeJNI.setnFormatID = env->GetMethodID(g_TagColumnItemNativeJNI.tagColumnItem_cls,"setnFormat","(S)V");
        if(g_TagColumnItemNativeJNI.setnFormatID == NULL){
            LOGE("%s: ====GameClient===not get java class tagColumnItem setnFormat======",__FUNCTION__);
            return;
        }
    }
}

void initServerInfoNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_ServerInfoNativeJNI.serverInfo_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_ServerInfoNativeJNI.serverInfo_cls == NULL){
        LOGE("%s: ===GameClient====not find java class CMD_GR_ServerInfo by java CMD_GR_ServerInfo======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GR_ServerInfo的构造函数ID
    if(g_ServerInfoNativeJNI.constructionID == NULL){
        g_ServerInfoNativeJNI.constructionID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls, "<init>", "()V");
        if(g_ServerInfoNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class CMD_GR_ServerInfo constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setwKindID方法ID
    if(g_ServerInfoNativeJNI.setwKindID == NULL){
        g_ServerInfoNativeJNI.setwKindID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setwKindID","(S)V");
        if(g_ServerInfoNativeJNI.setwKindID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_ServerInfo setwKindID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setwTableCount方法ID
    if(g_ServerInfoNativeJNI.setwTableCountID == NULL){
        g_ServerInfoNativeJNI.setwTableCountID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setwTableCount","(S)V");
        if(g_ServerInfoNativeJNI.setwTableCountID == NULL){
            LOGE("%s: ====GameClient===not get java class CMD_GR_ServerInfo setwTableCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setwChairCount方法ID
    if(g_ServerInfoNativeJNI.setwChairCountID == NULL){
        g_ServerInfoNativeJNI.setwChairCountID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setwChairCount","(S)V");
        if(g_ServerInfoNativeJNI.setwChairCountID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_ServerInfo setwChairCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setDwVideoAddr方法ID
    if(g_ServerInfoNativeJNI.setDwVideoAddrID == NULL){
        g_ServerInfoNativeJNI.setDwVideoAddrID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setDwVideoAddr","(I)V");
        if(g_ServerInfoNativeJNI.setDwVideoAddrID == NULL){
            LOGE("%s: ====GameClient===not get java class CMD_GR_ServerInfo setDwVideoAddr======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setwGameGenre方法ID
    if(g_ServerInfoNativeJNI.setwGameGenreID == NULL){
        g_ServerInfoNativeJNI.setwGameGenreID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setwGameGenre","(S)V");
        if(g_ServerInfoNativeJNI.setwGameGenreID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_ServerInfo setwGameGenre======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_ServerInfo中setCbHideUserInfo方法ID
    if(g_ServerInfoNativeJNI.setCbHideUserInfoID == NULL){
        g_ServerInfoNativeJNI.setCbHideUserInfoID = env->GetMethodID(g_ServerInfoNativeJNI.serverInfo_cls,"setCbHideUserInfo","(C)V");
        if(g_ServerInfoNativeJNI.setCbHideUserInfoID == NULL){
            LOGE("%s: ====GameClient===not get java class CMD_GR_ServerInfo setCbHideUserInfo======",__FUNCTION__);
            return;
        }
    }
}

void initTagUserDataNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_TagUserDataNativeJNI.userData_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_TagUserDataNativeJNI.userData_cls == NULL){
        LOGE("%s: ===GameClient====not find java class tagUserData by java tagUserData======",__FUNCTION__);
        return;
    }
    //获取java类tagUserData的构造函数ID
    if(g_TagUserDataNativeJNI.constructionID == NULL){
        g_TagUserDataNativeJNI.constructionID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls, "<init>", "()V");
        if(g_TagUserDataNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class tagUserData constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setwFaceID方法ID
    if(g_TagUserDataNativeJNI.setwFaceID == NULL){
        g_TagUserDataNativeJNI.setwFaceID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setwFaceID","(S)V");
        if(g_TagUserDataNativeJNI.setwFaceID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setwFaceID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwCustomFaceVer方法ID
    if(g_TagUserDataNativeJNI.setDwCustomFaceVerID == NULL){
        g_TagUserDataNativeJNI.setDwCustomFaceVerID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwCustomFaceVer","(J)V");
        if(g_TagUserDataNativeJNI.setDwCustomFaceVerID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setDwCustomFaceVer======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwUserID方法ID
    if(g_TagUserDataNativeJNI.setDwUserID == NULL){
        g_TagUserDataNativeJNI.setDwUserID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwUserID","(J)V");
        if(g_TagUserDataNativeJNI.setDwUserID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setDwUserID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwGroupID方法ID
    if(g_TagUserDataNativeJNI.setDwGroupID == NULL){
        g_TagUserDataNativeJNI.setDwGroupID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwGroupID","(J)V");
        if(g_TagUserDataNativeJNI.setDwGroupID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setDwGroupID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwGameID方法ID
    if(g_TagUserDataNativeJNI.setDwGameID == NULL){
        g_TagUserDataNativeJNI.setDwGameID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwGameID","(J)V");
        if(g_TagUserDataNativeJNI.setDwGameID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setDwGameID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwUserRight方法ID
    if(g_TagUserDataNativeJNI.setDwUserRightID == NULL){
        g_TagUserDataNativeJNI.setDwUserRightID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwUserRight","(J)V");
        if(g_TagUserDataNativeJNI.setDwUserRightID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setDwUserRight======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlLoveliness方法ID
    if(g_TagUserDataNativeJNI.setlLovelinessID == NULL){
        g_TagUserDataNativeJNI.setlLovelinessID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlLoveliness","(J)V");
        if(g_TagUserDataNativeJNI.setlLovelinessID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setlLoveliness======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setDwMasterRight方法ID
    if(g_TagUserDataNativeJNI.setDwMasterRightID == NULL){
        g_TagUserDataNativeJNI.setDwMasterRightID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setDwMasterRight","(J)V");
        if(g_TagUserDataNativeJNI.setDwMasterRightID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setDwMasterRight======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setSzName方法ID
    if(g_TagUserDataNativeJNI.setSzNameID == NULL){
        g_TagUserDataNativeJNI.setSzNameID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setSzName","(Ljava/lang/String;)V");
        if(g_TagUserDataNativeJNI.setSzNameID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setSzName======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setSzPassword方法ID
    if(g_TagUserDataNativeJNI.setSzPasswordID == NULL){
        g_TagUserDataNativeJNI.setSzPasswordID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setSzPassword","(Ljava/lang/String;)V");
        if(g_TagUserDataNativeJNI.setSzPasswordID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setSzPassword======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setSzPhone方法ID
    if(g_TagUserDataNativeJNI.setSzPhoneID == NULL){
        g_TagUserDataNativeJNI.setSzPhoneID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setSzPhone","(Ljava/lang/String;)V");
        if(g_TagUserDataNativeJNI.setSzPhoneID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setSzPhone======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setCbGender方法ID
    if(g_TagUserDataNativeJNI.setCbGenderID == NULL){
        g_TagUserDataNativeJNI.setCbGenderID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setCbGender","(B)V");
        if(g_TagUserDataNativeJNI.setCbGenderID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setCbGender======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setCbMemberOrder方法ID
    if(g_TagUserDataNativeJNI.setCbMemberOrderID == NULL){
        g_TagUserDataNativeJNI.setCbMemberOrderID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setCbMemberOrder","(B)V");
        if(g_TagUserDataNativeJNI.setCbMemberOrderID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setCbMemberOrder======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setCbMasterOrder方法ID
    if(g_TagUserDataNativeJNI.setCbMasterOrderID == NULL){
        g_TagUserDataNativeJNI.setCbMasterOrderID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setCbMasterOrder","(B)V");
        if(g_TagUserDataNativeJNI.setCbMasterOrderID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setCbMasterOrder======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlInsureScore方法ID
    if(g_TagUserDataNativeJNI.setlInsureScoreID == NULL){
        g_TagUserDataNativeJNI.setlInsureScoreID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlInsureScore","(J)V");
        if(g_TagUserDataNativeJNI.setlInsureScoreID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setlInsureScore======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlGameGold方法ID
    if(g_TagUserDataNativeJNI.setlGameGoldID == NULL){
        g_TagUserDataNativeJNI.setlGameGoldID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlGameGold","(J)V");
        if(g_TagUserDataNativeJNI.setlGameGoldID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setlGameGold======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlScore方法ID
    if(g_TagUserDataNativeJNI.setlScoreID == NULL){
        g_TagUserDataNativeJNI.setlScoreID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlScore","(J)V");
        if(g_TagUserDataNativeJNI.setlScoreID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setlScore======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlWinCount方法ID
    if(g_TagUserDataNativeJNI.setlWinCountID == NULL){
        g_TagUserDataNativeJNI.setlWinCountID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlWinCount","(J)V");
        if(g_TagUserDataNativeJNI.setlWinCountID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setlWinCount======",__FUNCTION__);
            return;
        }
    } //获取JAVA类tagUserData中setlLostCount方法ID
    if(g_TagUserDataNativeJNI.setlLostCountID == NULL){
        g_TagUserDataNativeJNI.setlLostCountID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlLostCount","(J)V");
        if(g_TagUserDataNativeJNI.setlLostCountID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setlLostCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlDrawCount方法ID
    if(g_TagUserDataNativeJNI.setlDrawCountID == NULL){
        g_TagUserDataNativeJNI.setlDrawCountID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlDrawCount","(J)V");
        if(g_TagUserDataNativeJNI.setlDrawCountID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setlDrawCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlFleeCount方法ID
    if(g_TagUserDataNativeJNI.setlFleeCountID == NULL){
        g_TagUserDataNativeJNI.setlFleeCountID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlFleeCount","(J)V");
        if(g_TagUserDataNativeJNI.setlFleeCountID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagUserData setlFleeCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlExperience方法ID
    if(g_TagUserDataNativeJNI.setlExperienceID == NULL){
        g_TagUserDataNativeJNI.setlExperienceID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlExperience","(J)V");
        if(g_TagUserDataNativeJNI.setlExperienceID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setlExperience======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setwMatchRank方法ID
    if(g_TagUserDataNativeJNI.setwMatchRankID == NULL){
        g_TagUserDataNativeJNI.setwMatchRankID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setwMatchRank","(S)V");
        if(g_TagUserDataNativeJNI.setwMatchRankID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setwMatchRank======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setlUsableScore方法ID
    if(g_TagUserDataNativeJNI.setlUsableScoreID == NULL){
        g_TagUserDataNativeJNI.setlUsableScoreID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setlUsableScore","(I)V");
        if(g_TagUserDataNativeJNI.setlUsableScoreID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setlUsableScore======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setwTableID方法ID
    if(g_TagUserDataNativeJNI.setwTableID == NULL){
        g_TagUserDataNativeJNI.setwTableID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setwTableID","(S)V");
        if(g_TagUserDataNativeJNI.setwTableID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setwTableID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setwChairID方法ID
    if(g_TagUserDataNativeJNI.setwChairID == NULL){
        g_TagUserDataNativeJNI.setwChairID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setwChairID","(S)V");
        if(g_TagUserDataNativeJNI.setwChairID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setwChairID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setCbUserStatus方法ID
    if(g_TagUserDataNativeJNI.setCbUserStatusID == NULL){
        g_TagUserDataNativeJNI.setCbUserStatusID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setCbUserStatus","(B)V");
        if(g_TagUserDataNativeJNI.setCbUserStatusID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setCbUserStatus======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagUserData中setCbIsOut方法ID
    if(g_TagUserDataNativeJNI.setCbIsOutID == NULL){
        g_TagUserDataNativeJNI.setCbIsOutID = env->GetMethodID(g_TagUserDataNativeJNI.userData_cls,"setCbIsOut","(Z)V");
        if(g_TagUserDataNativeJNI.setCbIsOutID == NULL){
            LOGE("%s: ====GameClient===not get java class tagUserData setCbIsOut======",__FUNCTION__);
            return;
        }
    }
}

void initUserStatusNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_UserStatusNativeJNI.userStatus_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_UserStatusNativeJNI.userStatus_cls == NULL){
        LOGE("%s: ===GameClient====not find java class CMD_GR_UserStatus by java CMD_GR_UserStatus======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GR_UserStatus的构造函数ID
    if(g_UserStatusNativeJNI.constructionID == NULL){
        g_UserStatusNativeJNI.constructionID = env->GetMethodID(g_UserStatusNativeJNI.userStatus_cls, "<init>", "()V");
        if(g_UserStatusNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class CMD_GR_UserStatus constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_UserStatus中setDwUserID方法ID
    if(g_UserStatusNativeJNI.setDwUserID == NULL){
        g_UserStatusNativeJNI.setDwUserID = env->GetMethodID(g_UserStatusNativeJNI.userStatus_cls,"setDwUserID","(J)V");
        if(g_UserStatusNativeJNI.setDwUserID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_UserStatus setDwUserID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_UserStatus中setwTableID方法ID
    if(g_UserStatusNativeJNI.setwTableID == NULL){
        g_UserStatusNativeJNI.setwTableID = env->GetMethodID(g_UserStatusNativeJNI.userStatus_cls,"setwTableID","(S)V");
        if(g_UserStatusNativeJNI.setwTableID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_UserStatus setwTableID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_UserStatus中setwChairID方法ID
    if(g_UserStatusNativeJNI.setwChairID == NULL){
        g_UserStatusNativeJNI.setwChairID = env->GetMethodID(g_UserStatusNativeJNI.userStatus_cls,"setwChairID","(S)V");
        if(g_UserStatusNativeJNI.setwChairID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_UserStatus setwChairID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_UserStatus中setCbUserStatus方法ID
    if(g_UserStatusNativeJNI.setCbUserStatusID == NULL){
        g_UserStatusNativeJNI.setCbUserStatusID = env->GetMethodID(g_UserStatusNativeJNI.userStatus_cls,"setCbUserStatus","(B)V");
        if(g_UserStatusNativeJNI.setCbUserStatusID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_UserStatus setCbUserStatus======",__FUNCTION__);
            return;
        }
    }
}

void initTableInfoNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_TableInfoNativeJNI.tableInfo_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_TableInfoNativeJNI.tableInfo_cls == NULL){
        LOGE("%s: ===GameClient====not find java class CMD_GR_TableInfo by java CMD_GR_TableInfo======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GR_TableInfo的构造函数ID
    if(g_TableInfoNativeJNI.constructionID == NULL){
        g_TableInfoNativeJNI.constructionID = env->GetMethodID(g_TableInfoNativeJNI.tableInfo_cls, "<init>", "()V");
        if(g_TableInfoNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class CMD_GR_TableInfo constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_TableInfo中setwTableCount方法ID
    if(g_TableInfoNativeJNI.setwTableCountID == NULL){
        g_TableInfoNativeJNI.setwTableCountID = env->GetMethodID(g_TableInfoNativeJNI.tableInfo_cls,"setwTableCount","(S)V");
        if(g_TableInfoNativeJNI.setwTableCountID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_TableInfo setwTableCount======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_TableInfo中setpTableStatus方法ID
    if(g_TableInfoNativeJNI.setpTableStatusID == NULL){
        g_TableInfoNativeJNI.setpTableStatusID = env->GetMethodID(g_TableInfoNativeJNI.tableInfo_cls,"setpTableStatus","([Lcom/android/chatframe/game/gametype/tagTableStatus;)V");
        if(g_TableInfoNativeJNI.setpTableStatusID == NULL){
            LOGE("%s: ====GameClient===not get java class CMD_GR_TableInfo setpTableStatus======",__FUNCTION__);
            return;
        }
    }
}

void initTagTableStatusNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_TagTableStatusNativeJNI.tagTableStatus_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_TagTableStatusNativeJNI.tagTableStatus_cls == NULL){
        LOGE("%s: ===GameClient====not find java class tagTableStatus by java tagTableStatus======",__FUNCTION__);
        return;
    }
    //获取java类tagTableStatus的构造函数ID
    if(g_TagTableStatusNativeJNI.constructionID == NULL){
        g_TagTableStatusNativeJNI.constructionID = env->GetMethodID(g_TagTableStatusNativeJNI.tagTableStatus_cls, "<init>", "()V");
        if(g_TagTableStatusNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class tagTableStatus constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagTableStatus中setbTableLock方法ID
    if(g_TagTableStatusNativeJNI.setbTableLockID == NULL){
        g_TagTableStatusNativeJNI.setbTableLockID = env->GetMethodID(g_TagTableStatusNativeJNI.tagTableStatus_cls,"setbTableLock","(B)V");
        if(g_TagTableStatusNativeJNI.setbTableLockID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagTableStatus setbTableLock======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类tagTableStatus中setbPlayStatus方法ID
    if(g_TagTableStatusNativeJNI.setbPlayStatusID == NULL){
        g_TagTableStatusNativeJNI.setbPlayStatusID = env->GetMethodID(g_TagTableStatusNativeJNI.tagTableStatus_cls,"setbPlayStatus","(B)V");
        if(g_TagTableStatusNativeJNI.setbPlayStatusID  == NULL){
            LOGE("%s: =====GameClient==not get java class tagTableStatus setbPlayStatus======",__FUNCTION__);
            return;
        }
    }
}

void initTableStatusNativeJNI(JNIEnv* env,const char* className)
{
    jclass tmp = env->FindClass(className);//FindClass获取的是局部引用,离开作用域会被系统自动回收
    g_TableStatusNativeJNI.tableStatus_cls = (jclass)env->NewGlobalRef(tmp);
    if(g_TableStatusNativeJNI.tableStatus_cls == NULL){
        LOGE("%s: ===GameClient====not find java class CMD_GR_TableStatus by java CMD_GR_TableStatus======",__FUNCTION__);
        return;
    }
    //获取java类CMD_GR_TableStatus的构造函数ID
    if(g_TableStatusNativeJNI.constructionID == NULL){
        g_TableStatusNativeJNI.constructionID = env->GetMethodID(g_TableStatusNativeJNI.tableStatus_cls, "<init>", "()V");
        if(g_TableStatusNativeJNI.constructionID == NULL){
            LOGE("%s: ===GameClient==not get java class CMD_GR_TableStatus constructionID======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_TableStatus中setbTableLock方法ID
    if(g_TableStatusNativeJNI.setbTableLockID == NULL){
        g_TableStatusNativeJNI.setbTableLockID = env->GetMethodID(g_TableStatusNativeJNI.tableStatus_cls,"setbTableLock","(B)V");
        if(g_TableStatusNativeJNI.setbTableLockID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_TableStatus setbTableLock======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_TableStatus中setbPlayStatus方法ID
    if(g_TableStatusNativeJNI.setbPlayStatusID == NULL){
        g_TableStatusNativeJNI.setbPlayStatusID = env->GetMethodID(g_TableStatusNativeJNI.tableStatus_cls,"setbPlayStatus","(B)V");
        if(g_TableStatusNativeJNI.setbPlayStatusID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_TableStatus setbPlayStatus======",__FUNCTION__);
            return;
        }
    }
    //获取JAVA类CMD_GR_TableStatus中setwTableID方法ID
    if(g_TableStatusNativeJNI.setwTableID == NULL){
        g_TableStatusNativeJNI.setwTableID = env->GetMethodID(g_TableStatusNativeJNI.tableStatus_cls,"setwTableID","(S)V");
        if(g_TableStatusNativeJNI.setwTableID  == NULL){
            LOGE("%s: =====GameClient==not get java class CMD_GR_TableStatus setwTableID======",__FUNCTION__);
            return;
        }
    }
}

