package com.android.chatframe.swig;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-6-22.
 */

public class SwigTest {
    static {
        System.loadLibrary("swigTest");
    }
    public static native String  SayHello();
}
