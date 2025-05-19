#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProgram.hpp"
#include "Renderer.hpp"
#include "Snake.hpp"
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
    Renderer m_renderer;
    ShaderProgram m_shader_program;
    ShaderProgram m_compute_program;

    int frames = 0, fps = 0;
    float m_delta_time = 0, delta_time_fps = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> then, now;
    bool should_run = true;
    bool init();

    Snake m_snake;
    Scene m_scene;
};
