#include <iostream>
#include "SnakeTracer.hpp"

int main() {
    SnakeTracer snake_tracer("src/glsl/vertex.glsl", "src/glsl/fragment.glsl", "src/glsl/compute.glsl");
    
    while (snake_tracer.is_running()){
        snake_tracer.draw_scene();
        snake_tracer.print_fps();
        snake_tracer.end_frame();
    }
    return 0;
}

