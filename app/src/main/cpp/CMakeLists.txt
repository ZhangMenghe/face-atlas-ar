cmake_minimum_required(VERSION 3.4.1)

SET(KERNEL_LIBRARIES
    GLESv3
    EGL
    android
    mediandk
    jnigraphics
    log
    m)

SET(GL_PIPELINE_FILES
        GLPipeline/Shader.cpp
        GLPipeline/Mesh.cpp
        GLPipeline/Texture.cpp)
SET(UTILS_FILES
        Utils/assetLoader.cpp)
SET(JNI_FILES
        Jnis/jni_interface.cpp
#        Jnis/jui_interface.cpp
        )
SET(RENDER_FILES
        Renderers/FaceMeshRenderer.cpp
        Renderers/lineRenderer.cpp
        Renderers/PlaneRenderer.cpp
        Renderers/PointCloudRenderer.cpp
        Renderers/backgroundRenderer.cpp
        Renderers/screenQuad.cpp)

#Import the ARCore library.
add_library(arcore SHARED IMPORTED)
set_target_properties(
        arcore PROPERTIES IMPORTED_LOCATION
        ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
        INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE}
)

include_directories(${ARCORE_INCLUDE})
include_directories(${GLM_INCLUDE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(native-lib
        SHARED
        arController.cpp
#        overlayController.cpp
#        Manager.cpp
        ${RENDER_FILES}
        ${JNI_FILES}

        ${GL_PIPELINE_FILES}
        ${UTILS_FILES}
        )

target_link_libraries(
        native-lib
        arcore
        ${KERNEL_LIBRARIES}
)
