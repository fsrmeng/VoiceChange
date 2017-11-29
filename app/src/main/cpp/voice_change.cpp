//
// Created by zp on 2017/11/29.
//
#include <stdlib.h>
#include <jni.h>
#include <unistd.h>
#include <fmod.hpp>

#define MODE_ORIGINAL   0
#define MODE_GIRL       1
#define MODE_UNCLE      2
#define MODE_PANIC      3
#define MODE_FUNNY      4
#define MODE_INTANGIBLE 5
using namespace FMOD;
extern "C"
JNIEXPORT void JNICALL
Java_com_zhangpan_myvoicechange_VoiceChangeUtils_fix(JNIEnv *env, jclass type_, jstring path_,
jint type) {
    System* system;
    void* extradriverdata = 0;
    Sound* sound;
    Channel* channel;
    DSP* dsp;
    bool playing = true;
    float frequency = 0;
    const char *path = env->GetStringUTFChars(path_, 0);
    try {
        //初始化
        System_Create(&system);
        system->init(32, FMOD_INIT_NORMAL, extradriverdata);
        //创建声音
        system->createSound(path, FMOD_DEFAULT, 0, &sound);
        switch (type) {
            case MODE_ORIGINAL:
                //原生播放
                system->playSound(sound, 0, false, &channel);
                break;
            case MODE_GIRL:
                //萝莉
                //FMOD_DSP_TYPE_PITCHSHIFT 提高或者降低音调的一种音效
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
                //设置音调的参数
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0);
                //播放萝莉声音
                system->playSound(sound, 0, false, &channel);
                //添加到channel
                channel->addDSP(0, dsp);
                break;
            case MODE_UNCLE:
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.8);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
            case MODE_PANIC:
                system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
                dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.5);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
            case MODE_FUNNY:
                //先playSound才能拿到原音频的频率
                system->playSound(sound, 0, false, &channel);
                channel->getFrequency(&frequency);
                channel->setFrequency(frequency * 1.8);
                break;
            case MODE_INTANGIBLE:
                system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
                dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
                dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0, dsp);
                break;
            default:
                break;
        }
        system->update();
        while (playing) {
            channel->isPlaying(&playing);
            usleep(1000 * 1000);
        }
    }catch (...) {
        goto end;
    }
    goto end;
end:
    sound->release();
    system->close();
    system->release();

    env->ReleaseStringUTFChars(path_, path);
}
