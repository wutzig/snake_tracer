#pragma once
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "Snake.hpp"
class Renderer{
public:
    Renderer();
    void initialize();
    void draw_scence(uint32_t shader_program_id);
    void use_compute_shader(uint32_t compute_program_id, Snake& snake, Scene& scene, float delta_time);
    void delete_buffers();
private:
    uint32_t vao_id, vertex_buffer_id, uvBufferID, element_buffer_id, textureID, frameBufferID;
};
