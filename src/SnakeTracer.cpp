#include "SnakeTracer.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

SnakeTracer::SnakeTracer(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path) {
    if (!init()) {
        std::cerr << "Terminating\n";
        std::exit(1);
    }

    m_shader_program.set_shaders(vertex_shader_path, fragment_shader_path, NULL);
    m_compute_program.set_shaders(NULL, NULL, compute_shader_path);

    m_renderer.initialize();
    
    m_scene.set_uniform_locations(m_compute_program.get_shader_program_id());
    m_scene.initialize(m_compute_program.get_shader_program_id());
    m_snake.set_uniform_locations(m_compute_program.get_shader_program_id());
    
    then = std::chrono::high_resolution_clock::now();
}

SnakeTracer::~SnakeTracer() {
    m_shader_program.delete_program();
    m_compute_program.delete_program();
    m_renderer.delete_buffers();
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SnakeTracer::end_frame() {
    glfwPollEvents();
    glfwSwapBuffers(window);
}
void SnakeTracer::draw_scene() {
    m_renderer.use_compute_shader(m_compute_program.get_shader_program_id(), m_snake, m_scene, m_delta_time);
    m_renderer.draw_scence(m_shader_program.get_shader_program_id());
}

bool SnakeTracer::is_running() {
    return !glfwWindowShouldClose(window) && should_run;
}
void SnakeTracer::print_fps() {
    frames++;
    now = std::chrono::high_resolution_clock::now();
    m_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
    then = now;
    delta_time_fps += m_delta_time;
    
    if(std::floor(delta_time_fps) >= 1000){
        std::cout << std::ceil((float)frames * 1000.0f / delta_time_fps) << " \r";
        delta_time_fps = 0.0f; 
        frames = 0;
    }
}
bool SnakeTracer::init() {
    if(!glfwInit()) {
        std::cerr << "init oops!\n";
        return false;
    }
    window = glfwCreateWindow(800, 600, "snake_tracer", NULL, NULL);
    if (!window) {
        std::cerr << "window oops!\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) {
        std::cerr << "glew oops!\n";
        glfwTerminate();
        return false;
    }
    
    // hacky way to capture 'this' (aka self) inside a callback lambda that can be converted to a function pointer 
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int, int action, int) {
        if(action == GLFW_PRESS) {
            SnakeTracer* self = static_cast<SnakeTracer*>(glfwGetWindowUserPointer(w));
            switch(key) {
            case GLFW_KEY_LEFT:
                self->m_snake.add_segment(0, -1.0);
                break;
            case GLFW_KEY_RIGHT:
                self->m_snake.add_segment(0, 1.0);
                break;
            case GLFW_KEY_UP:
                self->m_snake.add_segment(2, -1.0);
                break;
            case GLFW_KEY_DOWN:
                self->m_snake.add_segment(2, 1.0);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                self->m_snake.add_segment(1, -1.0);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                self->m_snake.add_segment(1, 1.0);
                break;
            case GLFW_KEY_G:
                self->m_snake.toggle_grow();
                break;
            case GLFW_KEY_ESCAPE:
                self->should_run = false;
                break;
        }
    }});
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int new_screen_width, int new_screen_height) {
        glViewport(0, 0, new_screen_width, new_screen_height);
    });
    glewExperimental = GL_TRUE;
    return true;
}

