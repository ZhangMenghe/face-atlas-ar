#include <Utils/assetLoader.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl32.h>
#include "jni_interface.h"

#include <android/bitmap.h>
#include <vector>
#include <Renderers/screenQuad.h>
#include <arController.h>

using namespace dvr;
namespace {
    arController* ar;
}
jint JNI_OnLoad(JavaVM *vm, void *) {
    g_vm = vm;
    return JNI_VERSION_1_6;
}

JNI_METHOD(jlong, JNIonCreate)(JNIEnv* env, jclass , jobject asset_manager){
    new assetLoader(AAssetManager_fromJava(env, asset_manager));
    ar = new arController;
    nativeAddr = getNativeClassAddr(ar);
    return nativeAddr;
}
JNI_METHOD(void, JNIonPause)(JNIEnv*, jclass){
//    vrController::instance()->onPause();
    ar->onPause();
}

JNI_METHOD(void, JNIonDestroy)(JNIEnv*, jclass){
//    vrController::instance()->onDestroy();
    ar->onDestroy();
    delete nativeApp(nativeAddr);
//    delete arController::instance();

    nativeAddr = 0;
}

JNI_METHOD(void, JNIonResume)(JNIEnv* env, jclass, jobject context, jobject activity){
    ar->onResume(env, context, activity);
}

JNI_METHOD(void, JNIonGlSurfaceCreated)(JNIEnv *, jclass){
    ar->onViewCreated();
//    overlayController::instance()->onViewCreated();
//    arController::instance()->onViewCreated();
}

JNI_METHOD(void, JNIonSurfaceChanged)(JNIEnv * env, jclass, jint rot, jint w, jint h){
//    manager->onViewChange(w, h);
//    nativeApp(nativeAddr)->onViewChange(w, h);
//    overlayController::instance()->onViewChange(w, h);
    ar->onViewChange(rot,w,h);
}

JNI_METHOD(void, JNIdrawFrame)(JNIEnv*, jclass){
    ar->onDraw();
}

JNIEnv *GetJniEnv() {
    JNIEnv *env;
    jint result = g_vm->AttachCurrentThread(&env, nullptr);
    return result == JNI_OK ? env : nullptr;
}

jclass FindClass(const char *classname) {
    JNIEnv *env = GetJniEnv();
    return env->FindClass(classname);
}