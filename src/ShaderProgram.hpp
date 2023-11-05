#pragma once
#include <glm/glm.hpp>
#include <vector>

const uint32_t MAX_SPHERES = 500;
const uint32_t MAX_CYLINDERS = 500;
const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 768;

struct UniformLocations{
    std::vector<uint32_t> sphere_positions;
    std::vector<uint32_t> sphere_colors;
    std::vector<uint32_t> sphere_radius2;
    std::vector<uint32_t> sphere_inv_radius;

    std::vector<uint32_t> cylinder_positions;
    std::vector<uint32_t> cylinder_axes;
    std::vector<uint32_t> cylinder_colors;
    std::vector<uint32_t> cylinder_radius;
    std::vector<uint32_t> cylinder_lengths;

    std::vector<uint32_t> square_positions;
    std::vector<uint32_t> square_colors;
    std::vector<uint32_t> square_normals;
    std::vector<uint32_t> square_u_dirs;
    std::vector<uint32_t> square_v_dirs;
    std::vector<uint32_t> square_lengths;

    uint32_t camera_dir, camera_up, camera_right, camera_position;
    uint32_t light_position, time, texture;
    uint32_t num_cylinders, num_spheres;
};
class ShaderProgram {
    public:
        ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path);
        uint32_t shader_program_id;
        void delete_program();
        UniformLocations locations;
    private:
        uint32_t load_shader(const char* source, uint32_t type);
        void load_uniforms();
        uint32_t vertex_shader_id;
        uint32_t fragment_shader_id;
        uint32_t compute_shader_id;
};
