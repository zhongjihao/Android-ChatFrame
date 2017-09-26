package com.android.chatframe.game.gametype;

/**
 * Created by SPREADTRUM\jihao.zhong on 17-9-7.
 */

public class GameConstant {
    //用户状态定义
    public class UserStatus{
        public final static int US_NULL		             = 0x00;		             //没有状态
        public final static int US_FREE		             = 0x01;					 //站立状态
        public final static int US_SIT		             = 0x02;				     //坐下状态
        public final static int US_READY	             = 0x03;					 //同意状态
        public final static int US_LOOKON	             = 0x04;					 //旁观状态
        public final static int US_PLAY		             = 0x05;					 //游戏状态
        public final static int US_OFFLINE	             = 0x06;					 //断线状态
    }

    //用户性别定义
    public class UserGender{
        public final static int GENDER_NULL			     =	0;
        public final static int GENDER_BOY			     =	1;
        public final static int GENDER_GIRL			     =	2;
    }

}
