#pragma once
#include <glm/glm.hpp>
#include <vector>

const uint32_t MAX_SPHERES = 500;
const uint32_t MAX_CYLINDERS = 500;
const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 768;

class ShaderProgram {
    public:
        ShaderProgram() = default;
        void set_shaders(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path);
        uint32_t get_shader_program_id();
        void delete_program();
    private:
        uint32_t load_shader(const char* source, uint32_t type);
        uint32_t vertex_shader_id;
        uint32_t fragment_shader_id;
        uint32_t compute_shader_id;
        uint32_t shader_program_id;
};
