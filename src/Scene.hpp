#pragma once

#include <vector>
#include <array>
#include <glm/glm.hpp>

class Scene
{
public:
    Scene();
    void set_uniform_locations(uint32_t shader_program_id);
    void initialize(uint32_t compute_program_id);
    void update(float delta_time);
private:
    void add_square(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec3 uDirection, glm::vec3 vDirection, glm::vec3 color);
    static glm::vec3 quat_rot(glm::vec3 point, glm::vec3 axis, float angle, glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0));
    
    struct UniformLocations {
        std::array<uint32_t, 3> square_positions;
        std::array<uint32_t, 3> square_colors;
        std::array<uint32_t, 3> square_normals;
        std::array<uint32_t, 3> square_u_dirs;
        std::array<uint32_t, 3> square_v_dirs;
        std::array<uint32_t, 3> square_lengths;

        uint32_t camera_dir, camera_up, camera_right, camera_position;
        uint32_t light_position, time, texture;
    } m_uniform_locations;
    
    glm::vec3 m_camera_dir;
    glm::vec3 m_camera_right;
    glm::vec3 m_camera_up;
    glm::vec3 m_camera_position;
    glm::vec3 m_light_position;

    std::array<glm::vec3, 3> m_square_positions;
    std::array<glm::vec3, 3> m_square_u_dirs;
    std::array<glm::vec3, 3> m_square_v_dirs;
    std::array<glm::vec3, 3> m_square_normals;
    std::array<glm::vec3, 3> m_square_colors;
    
    const float m_square_length = 1800.0f;
};