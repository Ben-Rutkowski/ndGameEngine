#include "application.hpp"
#include "edit_mesh.hpp"

void ndApp::startApp() {
    beginLoop();

    Shader vs(ShaderType::VERTEX, "modules/src/shaders/test/vertex.vs");
    Shader fs(ShaderType::FRAGMENT, "modules/src/shaders/test/fragment.fs");
    ShaderProgram face;
    face.attachShader(ShaderType::VERTEX, vs);
    face.attachShader(ShaderType::FRAGMENT, fs);
    face.linkProgram();

    Shader vs2(ShaderType::VERTEX, "modules/src/shaders/points/vertex.vs");
    Shader fs2(ShaderType::FRAGMENT, "modules/src/shaders/points/fragment.fs");
    ShaderProgram point;
    point.attachShader(ShaderType::VERTEX, vs2);
    point.attachShader(ShaderType::FRAGMENT, fs2);
    point.linkProgram();

    Shader vs3(ShaderType::VERTEX, "modules/src/shaders/lines/vertex.vs");
    Shader fs3(ShaderType::FRAGMENT, "modules/src/shaders/lines/fragment.fs");
    ShaderProgram line;
    line.attachShader(ShaderType::VERTEX, vs3);
    line.attachShader(ShaderType::FRAGMENT, fs3);
    line.linkProgram();

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
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    while (!window->shouldClose()) {
        startFrame();

        mesh.draw(point, line, face);

        pollEvents();
        endFrame();
    }
}