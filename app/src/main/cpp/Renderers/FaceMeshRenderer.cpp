#include <Renderers/screenQuad.h>
#include <Utils/assetLoader.h>
#include "FaceMeshRenderer.h"

FaceMeshRenderer::FaceMeshRenderer(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray((GLuint)vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    //with normal
//    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_POINT_NUM * 9, nullptr, GL_DYNAMIC_DRAW);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(4 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
//    glEnableVertexAttribArray(2);


    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_POINT_NUM * 3, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*MAX_TRIANGLES* 3, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

//    std::string VERT_SHADER, FRAG_SHADER;
//    if(assetLoader::instance()->LoadTextFileFromAssetManager("shaders/face_mesh.vert", &VERT_SHADER)
//    &&assetLoader::instance()->LoadTextFileFromAssetManager("shaders/face_mesh.frag", &VERT_SHADER))

        const std::string VERT_SHADER = "#version 310 es\n"
                                        "\n"
                                        "precision mediump float;\n"
                                        "layout (location = 0) in vec3 aPosition;\n"
                                        "\n"
                                        "uniform mat4 uMVP;\n"
                                        "void main(){\n"
                                        "    gl_PointSize = 10.0;\n"
                                        "    gl_Position = uMVP *vec4(aPosition, 1.0);\n"
                                        "}";
    const std::string FRAG_SHADER = "#version 310 es\n"
                                    "\n"
                                    "precision mediump float;\n"
                                    "out vec4 gl_FragColor;\n"
                                    "\n"
                                    "\n"
                                    "void main() {\n"
                                    "    gl_FragColor = vec4(1.0, .0,.0, 1.0);\n"
                                    "}";

    if(!shader_.AddShader(GL_VERTEX_SHADER, VERT_SHADER)
       ||!shader_.AddShader(GL_FRAGMENT_SHADER,  FRAG_SHADER)
       ||!shader_.CompileAndLink())
        LOGE("face mesh===Failed to create shader program===");
}
void FaceMeshRenderer::Draw(glm::mat4 model_mat, glm::mat4 view_mat, glm::mat4 proj_mat,
                                 int32_t vertice_num, const float* vertices,
                                 int32_t norm_num, const float* normals,
                                 int32_t uv_num, const float* uv,
                                 int32_t tri_num, const float* indices){


//if(point_num > 0){
//        draw_point_num = point_num;
//        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//        glBufferSubData(GL_ARRAY_BUFFER,0, point_num*4* sizeof(float),pointcloud);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }
//    GLuint sp = shader_.Use();
//    Shader::Uniform(sp, "uMVP", mvp);
//    glBindVertexArray(vao_);
//    glDrawArrays(GL_POINTS, 0, draw_point_num);
//    glBindVertexArray(0);
//    shader_.UnUse();
}
void FaceMeshRenderer::Draw(glm::mat4 mvp,
          int32_t vertice_num, const float* vertices,
          int32_t tri_num, const uint16_t* indices){
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER,0, vertice_num*3* sizeof(float), vertices);
//    glBufferSubData(GL_ARRAY_BUFFER,4* vertice_num* sizeof(float), tri_num*2* sizeof(float), indices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * tri_num * 3, indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint sp = shader_.Use();
    Shader::Uniform(sp, "uMVP", mvp);

    glBindVertexArray(vao_);
    glDrawElements(GL_LINE_STRIP, tri_num * 3, GL_UNSIGNED_SHORT, indices);
    glBindVertexArray(0);
    shader_.UnUse();
}