#include "ShaderProgram.hpp"
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>

void ShaderProgram::set_shaders(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path)
{
    std::cout << "creating shader program\n";
    shader_program_id = glCreateProgram();
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
}

uint32_t ShaderProgram::get_shader_program_id() {
    return shader_program_id;
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
