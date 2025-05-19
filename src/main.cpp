#include <iostream>
#include "SnakeTracer.hpp"

int main() {
    std::cout << "Welcome to SnakeTracer!\n";
    SnakeTracer snake_tracer("src/glsl/vertex.glsl", "src/glsl/fragment.glsl", "src/glsl/compute.glsl");
    
    while (snake_tracer.is_running()){
        snake_tracer.draw_scene();
        snake_tracer.print_fps();
        snake_tracer.end_frame();
    }
    return 0;
}

