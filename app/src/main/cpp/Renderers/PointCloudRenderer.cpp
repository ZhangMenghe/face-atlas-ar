#include <Renderers/screenQuad.h>
#include "PointCloudRenderer.h"

PointCloudRenderer::PointCloudRenderer(bool screen_baked)
:DRAW_TO_TEXTURE(screen_baked){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray((GLuint)vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_POINT_NUM * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    const std::string VERT_SHADER = "#version 310 es\n"
                                    "\n"
                                    "precision mediump float;\n"
                                    "layout (location = 0) in vec4 aPosition;\n"
                                    "\n"
                                    "uniform mat4 uMVP;\n"
                                    "void main(){\n"
                                    "    gl_PointSize = 10.0;\n"
                                    "    gl_Position = uMVP *vec4(aPosition.xyz, 1.0);\n"
                                    "}";
    const std::string FRAG_SHADER = "#version 310 es\n"
                                    "precision mediump float;\n"
                                    "out vec4 gl_FragColor;\n"
                                    "void main() {\n"
                                    "    gl_FragColor = vec4(1.0, 0.5, .0, 1.0);\n"
                                    "}";


    if(!shader_.AddShader(GL_VERTEX_SHADER, VERT_SHADER)
       ||!shader_.AddShader(GL_FRAGMENT_SHADER,  FRAG_SHADER)
       ||!shader_.CompileAndLink())
        LOGE("point cloud===Failed to create shader program===");
}
void PointCloudRenderer::Draw(glm::mat4 mvp, int point_num, const float * pointcloud){
    if(point_num > 0){
        draw_point_num = point_num;
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER,0, point_num*4* sizeof(float),pointcloud);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if(DRAW_TO_TEXTURE) draw_to_texture(mvp);
    else draw_scene(mvp);
}
void PointCloudRenderer::draw_to_texture(glm::mat4 mvp){
    if(!frame_buff_) Texture::initFBO(frame_buff_, screenQuad::instance()->getTex(), nullptr);
    glm::vec2 tsize = screenQuad::instance()->getTexSize();
    glViewport(0, 0, tsize.x, tsize.y);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buff_);
    glClear(GL_DEPTH_BUFFER_BIT);
    draw_scene(mvp);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void PointCloudRenderer::draw_scene(glm::mat4 mvp){
    GLuint sp = shader_.Use();
    Shader::Uniform(sp, "uMVP", mvp);
    glBindVertexArray(vao_);
    glDrawArrays(GL_POINTS, 0, draw_point_num);
    glBindVertexArray(0);
    shader_.UnUse();
}