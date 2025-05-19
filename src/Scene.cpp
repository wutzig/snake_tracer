#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.hpp"

Scene::Scene()
{
    m_camera_dir = glm::vec3(-0.48f, -0.51f, -glm::sqrt(1.0f - 0.48f * 0.48f - 0.51f * 0.51f));
    m_camera_right = glm::cross(m_camera_dir, glm::vec3(0.0f, 1.0f, 0.0f));
    m_camera_up = glm::cross(m_camera_right, m_camera_dir);
    m_camera_position = glm::vec3(2550.0f, 2700.0f, 3750.0f);
    
    add_square(0, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.4f, 0.78f, 0.8f));
    add_square(1, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.65f, 0.6f));
    add_square(2, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.8f, 0.90f, 0.7f));
    
    m_light_position = glm::vec3(1400.0f, 2800.0f, 1400.0f);
}

void Scene::add_square(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec3 u_dir, glm::vec3 v_dir, glm::vec3 color){
    m_square_positions[index] = std::move(position);
    m_square_normals[index] = std::move(normal);
    m_square_u_dirs[index] = std::move(u_dir);
    m_square_v_dirs[index] = std::move(v_dir);
    m_square_colors[index] = std::move(color);
}


void Scene::set_uniform_locations(uint32_t shader_program_id)
{
    for(int j = 0; j < 3; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "squares[" + j_str +"].pos";
        m_uniform_locations.square_positions[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
        glsl_name = "squares[" + j_str +"].color";
        m_uniform_locations.square_colors[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
        glsl_name = "squares[" + j_str +"].norm";
        m_uniform_locations.square_normals[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
        glsl_name = "squares[" + j_str +"].uDir";
        m_uniform_locations.square_u_dirs[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
        glsl_name = "squares[" + j_str +"].vDir";
        m_uniform_locations.square_v_dirs[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
        glsl_name = "squares[" + j_str +"].length";
        m_uniform_locations.square_lengths[j] = glGetUniformLocation(shader_program_id, glsl_name.c_str());
    }

    m_uniform_locations.camera_dir = glGetUniformLocation(shader_program_id, "camera_dir");
    m_uniform_locations.camera_up = glGetUniformLocation(shader_program_id, "camera_up");
    m_uniform_locations.camera_right = glGetUniformLocation(shader_program_id, "camera_right");
    m_uniform_locations.camera_position = glGetUniformLocation(shader_program_id, "camera_pos");
    m_uniform_locations.light_position = glGetUniformLocation(shader_program_id, "light");
    m_uniform_locations.time = glGetUniformLocation(shader_program_id, "time");
}

void Scene::initialize(uint32_t compute_program_id){
    glUseProgram(compute_program_id);
    for(int j=0; j < m_square_positions.size(); j++){
        glUniform3fv(m_uniform_locations.square_positions[j], 1, glm::value_ptr(m_square_positions[j]));
        glUniform3fv(m_uniform_locations.square_normals[j], 1, glm::value_ptr(m_square_normals[j]));
        glUniform3fv(m_uniform_locations.square_u_dirs[j], 1, glm::value_ptr(m_square_u_dirs[j]));
        glUniform3fv(m_uniform_locations.square_v_dirs[j], 1, glm::value_ptr(m_square_v_dirs[j]));
        glUniform3fv(m_uniform_locations.square_colors[j], 1, glm::value_ptr(m_square_colors[j]));
        glUniform1f(m_uniform_locations.square_lengths[j], m_square_length);
    }
    glUniform3fv(m_uniform_locations.camera_dir, 1, glm::value_ptr(m_camera_dir));
    glUniform3fv(m_uniform_locations.camera_up, 1, glm::value_ptr(m_camera_up));
    glUniform3fv(m_uniform_locations.camera_right, 1, glm::value_ptr(m_camera_right));
    glUniform3fv(m_uniform_locations.camera_position, 1, glm::value_ptr(m_camera_position));
    
    glUseProgram(GL_NONE);
}

void Scene::update(float delta_time)
{
    m_light_position = quat_rot(m_light_position,
        glm::vec3(0.0f, 1.0f, 0.0f),
        delta_time / 1000.f,
        glm::vec3(1100.0f, 2800.0f, 1100.0f)
    );

    glUniform3fv(m_uniform_locations.light_position, 1, glm::value_ptr(m_light_position));
    glUniform1f(m_uniform_locations.time, delta_time);
}

glm::vec3 Scene::quat_rot(glm::vec3 point, glm::vec3 axis, float angle, glm::vec3 origin){
    glm::vec3 r = axis * (float)glm::sin(angle / 2.0f);
    glm::vec3 P = point - origin;
    return point + glm::cross( 2.0f * r, glm::cross(r, P) + P * (float)glm::cos(angle/2.0f));;
}