#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.hpp"

class Renderer{
    public:
        Renderer();
        void draw_scence(uint32_t shader_program_id);
        void use_compute_shader(uint32_t compute_program_id);
        void delete_buffers();
        void set_uniform_locations(const UniformLocations& uniformLocations);
        void add_sphere(glm::vec3 position, glm::vec3 color, float radius);
        void add_square(glm::vec3 position, glm::vec3 normal, glm::vec3 uDirection, glm::vec3 vDirection, glm::vec3 color);
        void add_cylinder(glm::vec3 position, glm::vec3 axis, glm::vec3 color, float radius, float length);
        void add_segment(int orientation, float direction);
        void create_objects(uint32_t compute_program_id);
        float delta_time = 0.0f;
        bool grow = true;
    private:
        uint32_t vao_id, vertex_buffer_id, uvBufferID, element_buffer_id, textureID, frameBufferID,
            numSpheres = 0, numCylinders = 0;
        static glm::vec3 quat_rot(glm::vec3 point, glm::vec3 axis, float angle, glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0));
        UniformLocations locations;
        glm::vec3 camera_dir;
        glm::vec3 camera_right;
        glm::vec3 camera_up;
        glm::vec3 camera_position;

        std::vector<glm::vec3> cylinder_positions;
        std::vector<glm::vec3> cylinder_axes;
        std::vector<glm::vec3> cylinder_colors;
        std::vector<float> cylinder_radius;
        std::vector<float> cylinder_lengths;

        std::vector<glm::vec3> sphere_positions;
        std::vector<float> sphere_radius2;
        std::vector<float> sphere_inv_radius;
        std::vector<glm::vec3> sphere_colors;

        float square_length = 1800.0f;
        std::vector<glm::vec3> square_positions;
        std::vector<glm::vec3> square_u_dirs;
        std::vector<glm::vec3> square_v_dirs;
        std::vector<glm::vec3> square_normals;
        std::vector<glm::vec3> square_colors;

        glm::vec3 light_position;
};
