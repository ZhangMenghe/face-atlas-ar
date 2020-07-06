#include <GLPipeline/Primitive.h>
#include "backgroundRenderer.h"
#include <GLES2/gl2ext.h>
#include <Renderers/screenQuad.h>

backgroundRenderer::backgroundRenderer(bool screen_baked)
:DRAW_TO_TEXTURE(screen_baked){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray((GLuint)vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0,  8  *sizeof(float), kVertices);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id_);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    const std::string VERT_SHADER= "#version 310 es\n"
                                   "precision mediump float;\n"
                                   "layout (location = 0) in vec2 aPosition;\n"
                                   "layout (location = 1) in vec2 aTexcoord;\n"
                                   "\n"
                                   "out vec2 vTexcoord;\n"
                                   "\n"
                                   "void main(){\n"
                                   "    vTexcoord = aTexcoord.xy;\n"
                                   "    gl_Position = vec4(aPosition.xy, .0, 1.0);\n"
                                   "}";
    const std::string FRAG_SHADER= "#version 310 es\n"
                                   "\n"
                                   "#extension GL_OES_EGL_image_external_essl3 : enable\n"
                                   "#extension GL_OES_EGL_image_external : require\n"
                                   "\n"
                                   "precision mediump float;\n"
                                   "\n"
                                   "out vec4 gl_FragColor;\n"
                                   "in vec2 vTexcoord;\n"
                                   "\n"
                                   "uniform samplerExternalOES uSampler;\n"
                                   "\n"
                                   "void main(){\n"
                                   "    gl_FragColor = texture(uSampler, vTexcoord);\n"
                                   "}";


    if(!shader_.AddShader(GL_VERTEX_SHADER, VERT_SHADER)
       ||!shader_.AddShader(GL_FRAGMENT_SHADER,  FRAG_SHADER)
       ||!shader_.CompileAndLink())
        LOGE("ar-background===Failed to create opacity shader program===");
}
void backgroundRenderer::Draw(float * uvs_){
    //update uv data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 8*sizeof(float), 8 *sizeof(float), uvs_);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(DRAW_TO_TEXTURE) draw_to_texture();
    else draw_scene();
}
void backgroundRenderer::draw_to_texture(){
    if(!baked_dirty_) return;

    if(!frame_buff_) Texture::initFBO(frame_buff_, screenQuad::instance()->getTex(), nullptr);
    glm::vec2 tsize = screenQuad::instance()->getTexSize();
    glViewport(0, 0, tsize.x, tsize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buff_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_scene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    baked_dirty_ = false;
}
void backgroundRenderer::draw_scene(){
    GLuint sp = shader_.Use();
    glDepthMask(GL_FALSE);

    glActiveTexture(GL_TEXTURE0+acu::BACK_GROUND_AR_ID);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id_);
    Shader::Uniform(sp, "uSampler", acu::BACK_GROUND_AR_ID);

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    shader_.UnUse();
    glDepthMask(GL_TRUE);
}
GLuint backgroundRenderer::GetTextureId() const{ return texture_id_; }