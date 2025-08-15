#include "Snake.hpp"
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

Snake::Snake()
{
    add_sphere(glm::vec3(900.0f, 900.0f, 900.0f), glm::vec3(0.75f, 0.75f, 0.75f), 80.0f);
    
    float apple_radius = 80.f;
    m_apple_position = glm::vec3(450.0f, 450.0f, 450.0f);
    m_apple_color = glm::vec3(0.75f, 0.15f, 0.15f);
    m_apple_radius2 = apple_radius * apple_radius;
    m_apple_inv_radius = 1.0f / apple_radius;
}

void Snake::toggle_grow()
{
    m_grow = !m_grow;
}

void Snake::add_sphere(glm::vec3 position, glm::vec3 color, float radius){
    sphere_positions.push_back(position);
    sphere_radius2.push_back(radius * radius);
    sphere_inv_radius.push_back(1.0f / radius);
    sphere_colors.push_back(color);
}

void Snake::add_cylinder(glm::vec3 position, glm::vec3 axis, glm::vec3 color, float radius, float length){
    cylinder_positions.push_back(position);
    cylinder_axes.push_back(axis);
    cylinder_colors.push_back(color);
    cylinder_radius.push_back(radius);
    cylinder_lengths.push_back(length);

}
void Snake::add_segment(int orientation, float direction){
    if(cylinder_axes.size() > 0 && cylinder_axes.back()[orientation] != 0.0f) {
        return;
    }
    glm::vec3 axis = glm::vec3(0.0f, 0.0f, 0.0f);
    axis[orientation] = direction;
    
    float radius = 1.0 / sphere_inv_radius.front();
    add_cylinder(sphere_positions.back(), axis, sphere_colors.front(), radius, 0.0f);
    add_sphere(sphere_positions.back(), sphere_colors.front(), radius);
}

void Snake::update(float delta_time)
{
    if(cylinder_positions.size() > 0)
    {
        float distance = delta_time / 10.0f * m_speed;
        cylinder_lengths.back() += distance;
        sphere_positions.back() += distance * cylinder_axes.back();
        if(!m_grow)
        {
            sphere_positions.front() += distance * cylinder_axes.front();
            cylinder_positions.front() += distance * cylinder_axes.front();
            cylinder_lengths.front() -= distance;
            if(cylinder_lengths.front() <= 0.0){
                std::rotate(sphere_positions.begin(), sphere_positions.begin() + 1, sphere_positions.end());
                sphere_positions.pop_back();
                std::rotate(sphere_colors.begin(), sphere_colors.begin() + 1, sphere_colors.end());
                sphere_colors.pop_back();
                std::rotate(sphere_radius2.begin(), sphere_radius2.begin() + 1, sphere_radius2.end());
                sphere_radius2.pop_back();
                std::rotate(sphere_inv_radius.begin(), sphere_inv_radius.begin() + 1, sphere_inv_radius.end());
                sphere_inv_radius.pop_back();

                std::rotate(cylinder_positions.begin(), cylinder_positions.begin() + 1, cylinder_positions.end());
                cylinder_positions.pop_back();
                std::rotate(cylinder_axes.begin(), cylinder_axes.begin() + 1, cylinder_axes.end());
                cylinder_axes.pop_back();
                std::rotate(cylinder_colors.begin(), cylinder_colors.begin() + 1, cylinder_colors.end());
                cylinder_colors.pop_back();
                std::rotate(cylinder_lengths.begin(), cylinder_lengths.begin() + 1, cylinder_lengths.end());
                cylinder_lengths.pop_back();
                std::rotate(cylinder_radius.begin(), cylinder_radius.begin() + 1, cylinder_radius.end());
                cylinder_radius.pop_back();
            }
        }
    
        for(int j=0; j < cylinder_positions.size(); j++){
            glUniform3fv(m_uniform_locations.cylinder_positions[j], 1, glm::value_ptr(cylinder_positions[j]));
            glUniform3fv(m_uniform_locations.cylinder_axes[j], 1, glm::value_ptr(cylinder_axes[j]));
            glUniform3fv(m_uniform_locations.cylinder_colors[j], 1, glm::value_ptr(cylinder_colors[j]));
            glUniform1f(m_uniform_locations.cylinder_radius[j], cylinder_radius[j]);
            glUniform1f(m_uniform_locations.cylinder_lengths[j], cylinder_lengths[j]);
        }
    }
        
    glUniform1i(m_uniform_locations.num_spheres, sphere_positions.size());
    glUniform1i(m_uniform_locations.num_cylinders, cylinder_positions.size());

    for(int j=0; j < sphere_positions.size(); j++){
        glUniform3fv(m_uniform_locations.sphere_positions[j], 1, glm::value_ptr(sphere_positions[j]));
        glUniform3fv(m_uniform_locations.sphere_colors[j], 1, glm::value_ptr(sphere_colors[j]));
        glUniform1f(m_uniform_locations.sphere_radius2[j], sphere_radius2[j]);
        glUniform1f(m_uniform_locations.sphere_inv_radius[j], sphere_inv_radius[j]);
    }

    glUniform3fv(m_uniform_locations.apple_position, 1, glm::value_ptr(m_apple_position));
    glUniform3fv(m_uniform_locations.apple_color, 1, glm::value_ptr(m_apple_color));
    glUniform1f(m_uniform_locations.apple_radius2, m_apple_radius2);
    glUniform1f(m_uniform_locations.apple_inv_radius, m_apple_inv_radius);
}

void Snake::set_uniform_locations(uint32_t shader_program_id)
{
    for(int j = 0; j < MAX_SPHERES; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "spheres[" + j_str + "].center";
        m_uniform_locations.sphere_positions.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str +"].color";
        m_uniform_locations.sphere_colors.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str + "].radius2";
        m_uniform_locations.sphere_radius2.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str + "].inv_radius";
        m_uniform_locations.sphere_inv_radius.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
    }

    m_uniform_locations.apple_position = glGetUniformLocation(shader_program_id, "apple.center");
    m_uniform_locations.apple_color = glGetUniformLocation(shader_program_id, "apple.color");
    m_uniform_locations.apple_radius2 = glGetUniformLocation(shader_program_id, "apple.radius2");
    m_uniform_locations.apple_inv_radius = glGetUniformLocation(shader_program_id, "apple.inv_radius");

    for(int j = 0; j < MAX_CYLINDERS; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "cylinders[" + j_str +"].pos";
        m_uniform_locations.cylinder_positions.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].axis";
        m_uniform_locations.cylinder_axes.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].color";
        m_uniform_locations.cylinder_colors.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].radius";
        m_uniform_locations.cylinder_radius.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].length";
        m_uniform_locations.cylinder_lengths.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
    }

    m_uniform_locations.num_cylinders = glGetUniformLocation(shader_program_id, "numCylinders");
    m_uniform_locations.num_spheres = glGetUniformLocation(shader_program_id, "numSpheres");
}

