#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include <chrono>
class SnakeTracer {
public:
    SnakeTracer(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path);
    ~SnakeTracer();
    void print_fps();
    void draw_scene();
    void end_frame();
    bool is_running();
private:
    GLFWwindow* window;
    Renderer* renderer;
    ShaderProgram* shader_program;
    ShaderProgram* compute_program;

    int frames, fps;
    float delta_time, delta_time_fps;
    std::clock_t then, now;
    bool should_run = true;
    bool init();
};
