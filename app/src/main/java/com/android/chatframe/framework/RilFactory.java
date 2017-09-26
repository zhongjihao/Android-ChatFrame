package com.android.chatframe.framework;

import android.content.Context;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-5-3.
 */

public class RilFactory {
    private static final String LOG_TAG = "Ril-Factory";
    private static CommandsInterface sCommandsInterface = null;
    private static Context sContext;

    public static CommandsInterface makeRil(Context context) {
        if(sCommandsInterface == null){
            sContext = context;
            sCommandsInterface = new RIL(context,0);
        }
        return  sCommandsInterface;
    }

}
