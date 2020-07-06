#ifndef JNI_MAIN_H
#define JNI_MAIN_H

#include <jni.h>
#include <string>
#include <vector>
#include <arController.h>

#define JNI_METHOD(returnType, funcName)\
    JNIEXPORT returnType JNICALL        \
        Java_face_atlas_JNIInterface_##funcName

#define JUI_METHOD(returnType, funcName)\
    JNIEXPORT returnType JNICALL        \
        Java_face_atlas_JUIInterface_##funcName

namespace dvr{
    inline static JavaVM *g_vm = nullptr;
    inline jlong nativeAddr;

    inline jlong getNativeClassAddr(arController * native_controller){
        return reinterpret_cast<intptr_t>(native_controller);
    }
    inline arController * nativeApp(jlong ptr){
        return reinterpret_cast<arController *>(ptr);
    }
    inline std::string jstring2string(JNIEnv *env, jstring jStr){
        const char *cstr = env->GetStringUTFChars(jStr, NULL);
        std::string str = std::string(cstr);
        env->ReleaseStringUTFChars(jStr, cstr);
        return str;
    }
};
#endif