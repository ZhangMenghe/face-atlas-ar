
#ifndef ACUPOINTS_FACE_ATLAS_COMMON_H
#define ACUPOINTS_FACE_ATLAS_COMMON_H

#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif

#include <android/log.h>
#define GLSL_VERSION "#version 310 es\n"

#define TAG "ANDROID-HELPER"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#ifndef CHECK
#define CHECK(condition)                                                   \
      if (!(condition)) {                                                      \
        LOGE("===CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
      }
#endif  // CHECK

namespace acu{
    enum TEX_IDS{
        SCREEN_QUAD_TEX_ID=0,
        BACK_GROUND_AR_ID,
        PLANE_AR_ID,
    };
}

#endif //ACUPOINTS_FACE_ATLAS_COMMON_H
