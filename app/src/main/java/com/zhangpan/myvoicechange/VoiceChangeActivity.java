package com.zhangpan.myvoicechange;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;

import org.fmod.FMOD;

/**
 * Created by ZhangPan on 2017/11/29
 */

public class VoiceChangeActivity extends Activity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_voice_change);

        FMOD.init(this);
    }

    public void mFix(View view) {
        String path = "file:///android_asset/zp.wav";
        switch(view.getId()) {
            case R.id.iv_original:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_ORIGINAL);
                break;
            case R.id.iv_girl:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_GIRL);
                break;
            case R.id.iv_uncle:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_UNCLE);
                break;
            case R.id.iv_panic:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_PANIC);
                break;
            case R.id.iv_funny:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_FUNNY);
                break;
            case R.id.iv_intangible:
                VoiceChangeUtils.fix(path, VoiceChangeUtils.MODE_INTANGIBLE);
                break;
            default:
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }
}
