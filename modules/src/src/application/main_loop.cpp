#include "application.hpp"
#include "edit_mesh.hpp"

void ndApp::startApp() {
    beginLoop();

    ShaderProgram face = ShaderProgram::VF(
        "modules/src/shaders/edit_mesh/face.vs",
        "modules/src/shaders/edit_mesh/face.fs"
    );

    ShaderProgram point = ShaderProgram::VF(
        "modules/src/shaders/edit_mesh/point.vs",
        "modules/src/shaders/edit_mesh/point.fs"
    );

    ShaderProgram line = ShaderProgram::VF(
        "modules/src/shaders/edit_mesh/line.vs",
        "modules/src/shaders/edit_mesh/line.fs"
    );

    EditMesh mesh;

    Id4 bottom_p = {
        mesh.createPoint(vec4({-0.5f, -0.5f,  0.5f, 1.0f})),
        mesh.createPoint(vec4({ 0.5f, -0.5f,  0.5f, 1.0f})),
        mesh.createPoint(vec4({ 0.5f, -0.5f, -0.5f, 1.0f})),
        mesh.createPoint(vec4({-0.5f, -0.5f, -0.5f, 1.0f}))
    };

    Id4 top_p = {
        mesh.createPoint(vec4({-0.5f,  0.5f,  0.5f, 1.0f})),
        mesh.createPoint(vec4({ 0.5f,  0.5f,  0.5f, 1.0f})),
        mesh.createPoint(vec4({ 0.5f,  0.5f, -0.5f, 1.0f})),
        mesh.createPoint(vec4({-0.5f,  0.5f, -0.5f, 1.0f}))
    };

    Id4 bottom_e = {
        mesh.createEdge({bottom_p[0], bottom_p[1]}),
        mesh.createEdge({bottom_p[1], bottom_p[2]}),
        mesh.createEdge({bottom_p[2], bottom_p[3]}),
        mesh.createEdge({bottom_p[3], bottom_p[0]})
    };

    Id4 top_e = {
        mesh.createEdge({top_p[0], top_p[1]}),
        mesh.createEdge({top_p[1], top_p[2]}),
        mesh.createEdge({top_p[2], top_p[3]}),
        mesh.createEdge({top_p[3], top_p[0]})
    };

    Id4 side_e = {
        mesh.createEdge({bottom_p[0], top_p[0]}),
        mesh.createEdge({bottom_p[1], top_p[1]}),
        mesh.createEdge({bottom_p[2], top_p[2]}),
        mesh.createEdge({bottom_p[3], top_p[3]})
    };

    mesh.createQuad(
        {bottom_p[3], bottom_p[2], bottom_p[1], bottom_p[0]},
        {bottom_e[3], bottom_e[2], bottom_e[1], bottom_e[0]}
    );

    mesh.createQuad(
        {top_p[0], top_p[1], top_p[2], top_p[3]},
        {top_e[0], top_e[1], top_e[2], top_e[3]}
    );

    mesh.createQuad(
        {bottom_p[0], bottom_p[1], top_p[1], top_p[0]},
        {bottom_e[0], side_e[1], top_e[0], side_e[0]}
    );

    mesh.createQuad(
        {bottom_p[1], bottom_p[2], top_p[2], top_p[1]},
        {bottom_e[1], side_e[2], top_e[1], side_e[1]}
    );    

    mesh.createQuad(
        {bottom_p[2], bottom_p[3], top_p[3], top_p[2]},
        {bottom_e[2], side_e[3], top_e[2], side_e[2]}
    );    

    mesh.createQuad(
        {bottom_p[3], bottom_p[0], top_p[0], top_p[3]},
        {bottom_e[3], side_e[0], top_e[3], side_e[3]}
    );    

    mesh.load();
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_WIDTH);

    vec4 cpos;
    vec4 cright;
    vec4 cfront;
    mat4 view = mat4::iden();
    mat4 proj = mat4::projPer(math::rads(40.0f), 800.0f/600.0f, 0.1f, 100.0f);

    while (!window->shouldClose()) {
        startFrame();
        pollEvents();

        double time = window->getTime();
        float s = sin(time);
        float c = cos(time);

        cpos = vec4({2*c, c*0.5f, -2*s, 1.0f});
        cfront = vec4({c, c*0.5f, -s, 1.0f});
        cfront.scalarK(-1.0f, 3);
        cfront.normalizeK(3);
        cright = vec4::rightVec(cfront);
        view = mat4::view(cpos, cfront, cright);

        mesh.draw(point, line, face, view, proj);
        
        endFrame();
    }
}