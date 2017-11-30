# VoiceChange-mater
first commit
###前言
>首先我们来介绍**fmod**，fmod声音系统是为游戏开发者准备的**革命性音频引擎**，如今采用了fmod作为音频引擎的游戏包括Far Cry（孤岛惊魂）、Tom Clancy's Ghost Recon（幽灵行动），甚至著名的World Of Warcraft（魔兽争霸）。而我们此次的变声特效也是使用了fmod这个库。**fmod官网：**https://www.fmod.com，点击Download，下载针对Android的api
![1.png](http://upload-images.jianshu.io/upload_images/6069538-39710b9f6b8ca8ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

变声特效最后的效果图：

![2.png](http://upload-images.jianshu.io/upload_images/6069538-e15775b413f3348c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

###集成fmod，实现播放音频文件
废话不多说，直接上图：

![3.png](http://upload-images.jianshu.io/upload_images/6069538-158459c13862fbc6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

1. 创建一个支持C/C++的项目`MyVoiceChange`。
2. 将下载好的文件夹下的`api / lowlevel / inc` 下的头文件复制到项目中cpp下的`inc`文件夹（自己创建的）下。
3. 将下载好的文件夹下的`api / lowlevel / examples `下的`play_sound.cpp`复制到cpp下，`play_sound.cpp`中需要头文件`common.h`，故将`common.h`和`common.cpp`复制到cpp下，`common.h`需要的头文件有`common_platform.h`，同理将`common_platform.h`和`common_platform.cpp`复制到cpp下。
4. 将下载好的文件夹下的`api / lowlevel / lib `下的armeabi文件夹复制到项目中的libs目录下，将`fmod.jar`也复制到libs目录下，右键Add as library，在app的build.gradle中添加
```
externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "armeabi"
            }
        }
```
5. 在项目中的main文件夹下新建assets文件夹，将下载好的文件夹下的`api / lowlevel / examples / media` 文件夹下的`drumloop.wav、jaguar.wav、swish.wav`三个音频文件复制到assets中

6. 将inc文件夹下的都加上`inc/`，如`play_sound.cpp`中
```
#include "inc/fmod.hpp"
#include "common.h"
```
7. 将`MainActivity`复制到项目中，然后将删除关于权限的，**在清单文件中添加权限**
```
if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M)
        {
            String[] perms = { "android.permission.RECORD_AUDIO", "android.permission.WRITE_EXTERNAL_STORAGE" };
            if (checkSelfPermission(perms[0]) == PackageManager.PERMISSION_DENIED ||
                checkSelfPermission(perms[1]) == PackageManager.PERMISSION_DENIED)
            {
                requestPermissions(perms, 200);
            }
        }
```
```
<uses-permission android:name="android.permission.RECORD_AUDIO"/>
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
```
8. 将`MainActivity`中的加载库改为
```
static {
        System.loadLibrary("voice_change");
    }
```
**由于下面在CMakeLists中的target_link_libraries已经将fmod、fmodL与voice_change链接起来，故这里不需要重复添加加载fmod、fmodL。**
9. 在`common_platform.cpp`中将`Java_org_fmod_example_MainActivity_`改为`Java_com_zhangpan_myvoicechange_MainActivity_`
10. 配置CMakeLists
```
cmake_minimum_required(VERSION 3.4.1)

set(path_project E:/Demo/MyVoiceChange)

add_library(voice_change
            SHARED
            src/main/cpp/play_sound.cpp
            src/main/cpp/common_platform.cpp)

add_library(fmod
            SHARED
            IMPORTED)
set_target_properties(fmod
                      PROPERTIES IMPORTED_LOCATION
                      ${path_project}/app/libs/${ANDROID_ABI}/libfmod.so)
add_library(fmodL
            SHARED
            IMPORTED )
set_target_properties(fmodL
                      PROPERTIES IMPORTED_LOCATION
                      ${path_project}/app/libs/${ANDROID_ABI}/libfmodL.so)

include_directories(src/main/cpp/inc)

find_library(log-lib
             log)

target_link_libraries(voice_change
                      fmod
                      fmodL
                      ${log-lib})
```
编译运行报错

![4.png](http://upload-images.jianshu.io/upload_images/6069538-62b3759e61ec7856.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在app的build.gradle中的android中添加，**指定预编译so库的路径**
```
sourceSets {
        main {
            jniLibs.srcDirs = ['libs']
        }
    }
```
编译运行，成功播放音频文件。

![5.png](http://upload-images.jianshu.io/upload_images/6069538-158459c13862fbc6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

关于配置相关信息，可以查看中文官网地址（需要翻墙）：[向您的项目添加 C 和 C++ 代码](https://developer.android.com/studio/projects/add-native-code.html)

###仿QQ变声特效
新建一个cpp文件`voice_change.cpp`，将CMakeLists中第一个addLibrary中的`play_sound.cpp`换成`voice_change.cpp`，`src/main/cpp/play_sound.cpp`和`src/main/cpp/common_platform.cpp`删除，其余不变
```
add_library(voice_change
            SHARED
            src/main/cpp/voice_change.cpp)
```
此时可以将之前复制进来的`common.cpp、common.h、common_platform.cpp、common_platform.h、play_sound.cpp`都可以删除掉，当然了不删除也是没有关系的。
新建一个Activity文件`VoiceChangeActivity`，创建布局文件`activity_voice_change`，添加图片，样式
```
public class VoiceChangeActivity extends Activity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_voice_change);
    }
}
```
```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent" >

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_alignParentBottom="true"
        android:orientation="vertical"
        android:background="#FFF"
        android:paddingBottom="20dp"
        android:gravity="center_horizontal">

        <LinearLayout
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:orientation="horizontal"
            android:layout_marginTop="20dp">

            <LinearLayout 
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_original"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/record"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="原声"/>
            </LinearLayout>

            <LinearLayout
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_girl"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/luoli"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="萝莉"/>
            </LinearLayout>
            

            <LinearLayout
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_uncle"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/dashu"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="大叔"/>
            </LinearLayout>
        </LinearLayout>
        
        <LinearLayout
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:orientation="horizontal"
            android:layout_marginTop="20dp">

            <LinearLayout 
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_panic"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/jingsong"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="惊悚"/>
            </LinearLayout>

            <LinearLayout 
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_funny"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/gaoguai"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="搞怪"/>
            </LinearLayout>

            <LinearLayout 
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:orientation="vertical">
                <ImageView
                    android:id="@+id/iv_intangible"
	                style="@style/AudioImgStyle"
	                android:src="@mipmap/kongling"
	                android:onClick="mFix"/>
                <TextView 
                    style="@style/AudioTextStyle"
                    android:text="空灵"/>
            </LinearLayout>
        </LinearLayout>
    </LinearLayout>
</RelativeLayout>
```
```
<resources>
    <style name="AppBaseTheme" parent="android:Theme.Light"/>
    <style name="AppTheme" parent="AppBaseTheme"/>
    <style name="AudioImgStyle">
        <item name="android:layout_width">wrap_content</item>
        <item name="android:layout_height">wrap_content</item>
        <item name="android:layout_marginLeft">25dp</item>
        <item name="android:layout_marginRight">25dp</item>
    </style>
    <style name="AudioTextStyle">
        <item name="android:layout_width">wrap_content</item>
        <item name="android:layout_height">wrap_content</item>
        <item name="android:layout_marginTop">5dp</item>
        <item name="android:layout_gravity">center_horizontal</item>
    </style>
</resources>
```
在清单文件中将`MainActivity`替换成`VoiceChangeActivity`
创建`VoiceChangeUtils`
```
public class VoiceChangeUtils {
    public static final int MODE_ORIGINAL = 0;       //原声
    public static final int MODE_LITTLE_GIRL = 1;    //萝莉
    public static final int MODE_UNCLE = 2;           //大叔
    public static final int MODE_PANIC = 3;           //惊悚
    public static final int MODE_FUNNY = 4;           //搞怪
    public static final int MODE_INTANGIBLE = 5;      //空灵

    public static native void fix(String path, int type);

    static {
        System.loadLibrary("voice_change");
    }
}
```
`voice_change.cpp`**变声特效的核心实现：**
```
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
```
`VoiceChangeActivity`的调用:
```
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
```
编译运行，效果图如下：

![6.png](http://upload-images.jianshu.io/upload_images/6069538-e15775b413f3348c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

###展望

**喜欢本篇博客的简友们，就请来一波点赞，您的每一次关注，将成为我前进的动力，谢谢！**
