#ifndef RENDERERS_POINTCLOUD_RENDERER_H
#define RENDERERS_POINTCLOUD_RENDERER_H
#include <GLPipeline/Shader.h>
class PointCloudRenderer {
public:
    PointCloudRenderer(bool screen_baked = false);
    ~PointCloudRenderer() = default;

    virtual void Draw(glm::mat4 mvp, int point_num, const float * pointcloud);

protected:
    Shader shader_;
    GLuint frame_buff_ = 0;
    GLuint vao_, vbo_;
    bool DRAW_TO_TEXTURE;
    int draw_point_num = 0;
    const int MAX_POINT_NUM = 200;

    virtual void draw_to_texture(glm::mat4 mvp);
    virtual void draw_scene(glm::mat4 mvp);
};

#endif