#include "Renderer.hpp"
#include "Snake.hpp"
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <string>

Renderer::Renderer(){}

void Renderer::initialize() {
    vao_id = 0;
    glCreateVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    vertex_buffer_id = 0;
    glCreateBuffers(1, &vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    
    const float vertices[] = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glCreateBuffers(1, &element_buffer_id);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);
    const unsigned short elements[6] = {0, 1, 2, 0, 2, 3};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, GL_NONE);

    glCreateBuffers(1, &uvBufferID);
    glEnableVertexAttribArray(1);
    const float texCoords[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, GL_NONE);

    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, GL_NONE);
    glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    int work_grp_cnt[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
    std::cout << "max global (total) work group size x: " << work_grp_cnt[0]
        << ", " << work_grp_cnt[1] << ", " << work_grp_cnt[2] << "\n";

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_cnt[2]);
    std::cout << "max local (total) work group size x: " <<work_grp_cnt[0]
        << ", " << work_grp_cnt[1] << ", " << work_grp_cnt[2] << "\n";
    
    int work_grp_inv;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
    std::cout << "max local work group invocations: " << work_grp_inv << "\n";
    
    int max_uniform_count;
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_uniform_count);
    std::cout << "max uniforms in compute shader: " << work_grp_inv << "\n";
    
    int output = 0;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &output);
    std::cout << "max buffer bindings: " << output << "\n";
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &output);
    std::cout << "max block size: " << output << "\n";
    glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &output);
    std::cout << "max vertex blocks: " << output << "\n";
    glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &output);
    std::cout << "max frame blocks: " << output << "\n";
    glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &output);
    std::cout << "max compute blocks: " << output << "\n";
    glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &output);
    std::cout << "max total blocks: " << output << "\n";

    glBindVertexArray(GL_NONE);
}
void Renderer::draw_scence(uint32_t shader_program_id){
    glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_program_id);

    glBindVertexArray(vao_id);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, GL_NONE);
    glDisableVertexAttribArray(0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);
    glUseProgram(GL_NONE);
}

void Renderer::use_compute_shader(uint32_t compute_program_id, Snake& snake, Scene& scene, float delta_time){
    glUseProgram(compute_program_id);
    snake.update(delta_time);
    scene.update(delta_time);

    glDispatchCompute((unsigned int)(WIDTH / 4), (unsigned int)(HEIGHT / 4), 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
void Renderer::delete_buffers(){
    glDeleteBuffers(1, &vertex_buffer_id);
    glDeleteBuffers(1, &element_buffer_id);
}
