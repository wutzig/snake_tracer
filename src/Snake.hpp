#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ShaderProgram.hpp"

class Snake
{
public:
    Snake();
    void add_segment(int orientation, float direction);
    void toggle_grow();
    void update(float delta_time);
    void set_uniform_locations(uint32_t shader_prgram_id);
private:
    void add_sphere(glm::vec3 position, glm::vec3 color, float radius);
    void add_cylinder(glm::vec3 position, glm::vec3 axis, glm::vec3 color, float radius, float length);
    std::vector<glm::vec3> cylinder_positions;
    std::vector<glm::vec3> cylinder_axes;
    std::vector<glm::vec3> cylinder_colors;
    std::vector<float> cylinder_radius;
    std::vector<float> cylinder_lengths;

    std::vector<glm::vec3> sphere_positions;
    std::vector<float> sphere_radius2;
    std::vector<float> sphere_inv_radius;
    std::vector<glm::vec3> sphere_colors;

    glm::vec3 m_apple_position;
    float m_apple_radius2;
    float m_apple_inv_radius;
    glm::vec3 m_apple_color;

    uint32_t numSpheres = 0, numCylinders = 0;
    bool m_grow = true;
    float m_speed = 4.0f;

    struct UniformLocations {
        uint32_t apple_position;
        uint32_t apple_color;
        uint32_t apple_radius2;
        uint32_t apple_inv_radius;

        std::vector<uint32_t> sphere_positions;
        std::vector<uint32_t> sphere_colors;
        std::vector<uint32_t> sphere_radius2;
        std::vector<uint32_t> sphere_inv_radius;

        std::vector<uint32_t> cylinder_positions;
        std::vector<uint32_t> cylinder_axes;
        std::vector<uint32_t> cylinder_colors;
        std::vector<uint32_t> cylinder_radius;
        std::vector<uint32_t> cylinder_lengths;

        uint32_t num_spheres;
        uint32_t num_cylinders;

    } m_uniform_locations;
};