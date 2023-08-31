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
    Id tr = mesh.createPoint(vec4({ 0.5f,  0.5f, 0.0f, 1.0f}));
    Id tl = mesh.createPoint(vec4({-0.5f,  0.5f, 0.0f, 1.0f}));

    Id b = mesh.createEdge({bl, br});
    Id r = mesh.createEdge({br, tr});
    Id t = mesh.createEdge({tr, tl});
    Id l = mesh.createEdge({tl, bl});

    mesh.createQuad(
        {bl, br, tr, tl},
        {b, r, t, l}
    );

    mesh.load();
    mesh.translate(vec4({0.0f, 0.0f, -1.0f, 1.0f}));
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    vec4 cpos;
    vec4 cright;
    vec4 cfront;
    mat4 view = mat4::iden();

    while (!window->shouldClose()) {
        startFrame();

        double time = window->getTime();
        float place = sin(time);

        cfront = vec4({-0.2f, place/2.0f, -1.0f, 1.0f});
        cpos = vec4({0.0f, 0.0f, 1.0f + place, 1.0f});
        cfront.normalizeK(3);
        cright = vec4::rightVec(cfront);
        view = mat4::view(cpos, cfront, cright);

        mesh.draw(point, line, face, view);

        pollEvents();
        endFrame();
    }
}