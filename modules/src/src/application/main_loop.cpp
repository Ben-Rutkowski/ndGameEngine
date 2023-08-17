#include "application.hpp"
#include "shader_program.hpp"

void ndApp::startApp() {
    Shader vertex(ShaderType::VERTEX, "modules/src/shaders/test.vs");
    Shader fragment(ShaderType::FRAGMENT, "modules/src/shaders/test.fs");

    ShaderProgram program;
    program.attachShader(ShaderType::VERTEX, vertex);
    program.attachShader(ShaderType::FRAGMENT, fragment);
    program.linkProgram();

    float coords[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    beginLoop();
    while (!window->shouldClose()) {
        startFrame();

        program.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        pollEvents();
        endFrame();
    }
}