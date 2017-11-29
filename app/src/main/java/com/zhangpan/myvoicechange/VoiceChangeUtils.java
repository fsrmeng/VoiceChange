package com.zhangpan.myvoicechange;

/**
 * Created by ZhangPan on 2017/11/29
 */

public class VoiceChangeUtils {
    public static final int MODE_ORIGINAL = 0;       //原声
    public static final int MODE_GIRL = 1;    //萝莉
    public static final int MODE_UNCLE = 2;           //大叔
    public static final int MODE_PANIC = 3;           //惊悚
    public static final int MODE_FUNNY = 4;           //搞怪
    public static final int MODE_INTANGIBLE = 5;      //空灵

    public static native void fix(String path, int type);

    static {
        System.loadLibrary("voice_change");
    }
}
