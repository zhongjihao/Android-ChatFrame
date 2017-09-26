//
// Created by SPREADTRUM\jihao.zhong on 17-6-22.
//

#include "com_android_chatframe_swig_SwigTest.h"

JNIEXPORT jstring JNICALL Java_com_android_chatframe_swig_SwigTest_SayHello(JNIEnv *env, jclass jcls)
{
    return env ->NewStringUTF("SWIG测试");
}
