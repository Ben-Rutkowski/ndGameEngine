#include "application.hpp"
#include "simple_static_mesh.hpp"

void ndApp::startApp() {
    Shader vertex(ShaderType::VERTEX, "modules/src/shaders/test.vs");
    Shader fragment(ShaderType::FRAGMENT, "modules/src/shaders/test.fs");

    ShaderProgram program;
    program.attachShader(ShaderType::VERTEX, vertex);
    program.attachShader(ShaderType::FRAGMENT, fragment);
    program.linkProgram();

    Vertex point0;
    Vertex point1;
    Vertex point2;

    point0.position = vec3(-0.5f, -0.5f, 0.0f);
    point0.normal   = vec3(0.0f, 0.0f, 0.0f);
    point0.texcoord = vec2(0.0f, 0.0f);

    point1.position = vec3(0.5f, -0.5f, 0.0f);
    point1.normal   = vec3(0.0f, 0.0f, 0.0f);
    point1.texcoord = vec2(0.0f, 0.0f);

    point2.position = vec3(0.0f, 0.5f, 0.0f);
    point2.normal   = vec3(0.0f, 0.0f, 0.0f);
    point2.texcoord = vec2(0.0f, 0.0f);

    Verts my_verts = {point0, point1, point2};
    Ints  my_ints  = {0, 1, 2};

    SimpleStaticMesh mesh(my_verts, my_ints);
    mesh.loadData();

    // unsigned int VAO, VBO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    beginLoop();
    while (!window->shouldClose()) {
        startFrame();

        // program.use();
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        mesh.draw(program);

        pollEvents();
        endFrame();
    }
}