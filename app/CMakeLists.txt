cmake_minimum_required(VERSION 3.4.1)

set(path_project E:/Demo/MyVoiceChange)

add_library(voice_change
            SHARED
            src/main/cpp/voice_change.cpp)

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

