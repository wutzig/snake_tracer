#include "SnakeTracer.hpp"
#include <iostream>
#include <functional>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

SnakeTracer::SnakeTracer(const char* vertex_shader_path, const char* fragment_shader_path, const char* compute_shader_path) {
    if (!init()) {
        std::cerr << "Terminating\n";
        std::exit(1);
    }

    shader_program = new ShaderProgram(vertex_shader_path, fragment_shader_path, NULL);
    compute_program = new ShaderProgram(NULL, NULL, compute_shader_path);

    renderer = new Renderer();
    renderer->update_uniform_locations(compute_program->locations);
    renderer->create_objects(compute_program->shader_program_id);
    
    then = std::clock();
}

SnakeTracer::~SnakeTracer() {
    shader_program->delete_program();
    delete shader_program;

    compute_program->delete_program();
    delete compute_program;

    renderer->delete_buffers();
    delete renderer;
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void SnakeTracer::end_frame() {
    glfwPollEvents();
    glfwSwapBuffers(window);
}
void SnakeTracer::draw_scene() {
    renderer->use_compute_shader(compute_program->shader_program_id);
    renderer->draw_scence(shader_program->shader_program_id);
}

bool SnakeTracer::is_running() {
    return !glfwWindowShouldClose(window) && should_run;
}
void SnakeTracer::print_fps() {
    frames++;
    now = std::clock();
    delta_time = now - then;
    then = now;
    delta_time_fps += delta_time;
    renderer->delta_time = delta_time;
    
    if(glm::floor(delta_time_fps) >= 1000){
        std::cout << glm::ceil((float)frames * 1000.0f / delta_time_fps) << " \r";
        delta_time_fps = 0.0f; 
        frames = 0;
    }
}
bool SnakeTracer::init() {
    if(!glfwInit()) {
        std::cerr << "init oops!\n";
        return false;
    }
    window = glfwCreateWindow(800, 600, "My Title", NULL, NULL);
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
                self->renderer->add_segment(0, -1.0);
                break;
            case GLFW_KEY_RIGHT:
                self->renderer->add_segment(0, 1.0);
                break;
            case GLFW_KEY_UP:
                self->renderer->add_segment(2, -1.0);
                break;
            case GLFW_KEY_DOWN:
                self->renderer->add_segment(2, 1.0);
                break;
            case GLFW_KEY_LEFT_CONTROL:
                self->renderer->add_segment(1, -1.0);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                self->renderer->add_segment(1, 1.0);
                break;
            case GLFW_KEY_G:
                self->renderer->grow = !self->renderer->grow;
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

