#include "application.hpp"
#include "shader_program.hpp"

void ndApp::startApp() {
    Shader vertex(ShaderType::VERTEX, "modules/src/shaders/test.vs");
    Shader fragment(ShaderType::FRAGMENT, "modules/src/shaders/test.fs");

    ShaderProgram program;
    program.attachShader(ShaderType::VERTEX, vertex);
    program.attachShader(ShaderType::FRAGMENT, fragment);
    program.linkProgram();

    while (!window->shouldClose()) {
        startFrame();

        event_manager.pollEvents();

        glfwPollEvents();
    }
}