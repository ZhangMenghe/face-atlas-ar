#ifndef RENDERERS_FACEMESH_RENDERER_H
#define RENDERERS_FACEMESH_RENDERER_H
#include <GLPipeline/Shader.h>
class FaceMeshRenderer {
public:
    FaceMeshRenderer();
    void Draw(glm::mat4 model_mat, glm::mat4 view_mat, glm::mat4 proj_mat,
                      int32_t vertice_num, const float* vertices,
                      int32_t norm_num, const float* normals,
                      int32_t uv_num, const float* uv,
                      int32_t tri_num, const float* indices);
    void Draw(glm::mat4 mvp,
              int32_t vertice_num, const float* vertices,
              int32_t tri_num, const uint16_t* indices);

protected:
    Shader shader_;
    GLuint vao_, vbo_, ebo_;
    const int MAX_POINT_NUM = 500;
    const int MAX_TRIANGLES = 900;
};

#endif