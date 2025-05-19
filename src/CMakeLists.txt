cmake_minimum_required(VERSION 3.21)
project(snake_tracer CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)

add_executable(snake_tracer
    main.cpp
    Renderer.cpp
    Scene.cpp
    Snake.cpp
    SnakeTracer.cpp
    ShaderProgram.cpp
)
target_link_libraries(snake_tracer
    glfw
    GLEW::GLEW
    opengl::opengl
    glm::glm
)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()