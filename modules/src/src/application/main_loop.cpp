#include "application.hpp"
#include "edit_mesh.hpp"

void ndApp::startApp() {
    beginLoop();

    Shader vs(ShaderType::VERTEX, "modules/src/shaders/test/vertex.vs");
    Shader fs(ShaderType::FRAGMENT, "modules/src/shaders/test/fragment.fs");
    ShaderProgram program;
    program.attachShader(ShaderType::VERTEX, vs);
    program.attachShader(ShaderType::FRAGMENT, fs);
    program.linkProgram();

    EditMesh mesh;
    Id bl = mesh.createPoint(vec4({-0.5f, -0.5f, 0.0f, 1.0f}));
    Id br = mesh.createPoint(vec4({ 0.5f, -0.5f, 0.0f, 1.0f}));
    Id t  = mesh.createPoint(vec4({ 0.0f,  0.5f, 0.0f, 1.0f}));

    Id b = mesh.createEdge({bl, br});
    Id r = mesh.createEdge({br, t});
    Id l = mesh.createEdge({t, bl});

    Id tri = mesh.createTri(
        {bl, br, t},
        {b, r, l}
    );

    mesh.load();

    while (!window->shouldClose()) {
        startFrame();

        mesh.draw(program);

        pollEvents();
        endFrame();
    }
}