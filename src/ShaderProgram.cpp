#include "ShaderProgram.hpp"
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>

ShaderProgram::ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path) : 
        shader_program_id(glCreateProgram())
{
    std::cout << "creating shader program\n";
    if(vertex_shader_path != NULL){
        vertex_shader_id = load_shader(vertex_shader_path, GL_VERTEX_SHADER);
        glAttachShader(shader_program_id, vertex_shader_id);
        glBindAttribLocation(shader_program_id, 0, "vertexPosition");
        glBindAttribLocation(shader_program_id, 1, "textureCoordinate");
    }
    if(fragment_shader_path != NULL){
        fragment_shader_id = load_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
        glAttachShader(shader_program_id, fragment_shader_id);
    
    }
    if(compute_shader_path != NULL){
        compute_shader_id = load_shader(compute_shader_path, GL_COMPUTE_SHADER);
        glAttachShader(shader_program_id, compute_shader_id);
    }
    
    glLinkProgram(shader_program_id);
    int is_linked = 0;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &is_linked);
    if(is_linked == GL_FALSE){
        int max_length = 0;
        glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &max_length);
        char* err = new char[max_length];
        glGetProgramInfoLog(shader_program_id, max_length, &max_length, err);
        std::cerr << err;
        delete err;
        glDeleteProgram(shader_program_id);
        return;
    }
    load_uniforms();
}

const UniformLocations& ShaderProgram::get_uniform_locations() const {
    return locations;
}
void ShaderProgram::delete_program(){
    glDetachShader(shader_program_id, vertex_shader_id);
    glDeleteShader(vertex_shader_id);
    glDetachShader(shader_program_id, fragment_shader_id);
    glDeleteShader(fragment_shader_id);
    glDetachShader(shader_program_id, compute_shader_id);
    glDeleteShader(compute_shader_id);
    glUseProgram(0);
    glDeleteProgram(shader_program_id);
}
uint32_t ShaderProgram::load_shader(const char* path, unsigned int type){
    std::string shaderString;
    std::ifstream source_file( path );
    std::cout << "Trying to read " << path << std::endl;
    if( source_file ){
        shaderString.assign(
            std::istreambuf_iterator<char>( source_file ),
            std::istreambuf_iterator<char>()
        );
        std::cout << "success" << std::endl;
    }
    else {
        std::cerr << "cannot open file " << path << std::endl;
    }
    const char* source = shaderString.c_str();
    const int shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, NULL);
    glCompileShader(shader_id);
    int is_compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
    if(is_compiled == GL_FALSE){
        int max_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);
        char* err = new char[max_length];
        glGetShaderInfoLog(shader_id, max_length, &max_length, err);
        std::cerr << err;
        delete err;
        glDeleteShader(shader_id);
    }

    return shader_id;
}
void ShaderProgram::load_uniforms(){
    for(int j = 0; j < MAX_SPHERES; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "spheres[" + j_str + "].center";
        locations.sphere_positions.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str +"].color";
        locations.sphere_colors.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str + "].radius2";
        locations.sphere_radius2.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "spheres[" + j_str + "].inv_radius";
        locations.sphere_inv_radius.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
    }

    for(int j = 0; j < MAX_CYLINDERS; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "cylinders[" + j_str +"].pos";
        locations.cylinder_positions.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].axis";
        locations.cylinder_axes.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].color";
        locations.cylinder_colors.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].radius";
        locations.cylinder_radius.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "cylinders[" + j_str + "].length";
        locations.cylinder_lengths.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
    }

    for(int j = 0; j < 3; j++){
        std::string j_str = std::to_string(j);
        std::string glsl_name = "squares[" + j_str +"].pos";
        locations.square_positions.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "squares[" + j_str +"].color";
        locations.square_colors.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "squares[" + j_str +"].norm";
        locations.square_normals.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "squares[" + j_str +"].uDir";
        locations.square_u_dirs.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "squares[" + j_str +"].vDir";
        locations.square_v_dirs.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
        glsl_name = "squares[" + j_str +"].length";
        locations.square_lengths.push_back(glGetUniformLocation(shader_program_id, glsl_name.c_str()));
    }

    locations.camera_dir = glGetUniformLocation(shader_program_id, "camera_dir");
    locations.camera_up = glGetUniformLocation(shader_program_id, "camera_up");
    locations.camera_right = glGetUniformLocation(shader_program_id, "camera_right");
    locations.camera_position = glGetUniformLocation(shader_program_id, "camera_pos");
    locations.light_position = glGetUniformLocation(shader_program_id, "light");
    locations.time = glGetUniformLocation(shader_program_id, "time");
    locations.num_cylinders = glGetUniformLocation(shader_program_id, "numCylinders");
    locations.num_spheres = glGetUniformLocation(shader_program_id, "numSpheres");
    locations.texture = glGetUniformLocation(shader_program_id, "renderedTexture");
}