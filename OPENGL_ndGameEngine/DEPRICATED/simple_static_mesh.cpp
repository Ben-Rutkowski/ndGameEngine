#include "simple_static_mesh.hpp"

// === Constructors and Initialization ===
SimpleStaticMesh::SimpleStaticMesh()
    :VertexBufferInterface() {}

SimpleStaticMesh::SimpleStaticMesh(Verts vertices_in, Ints indices_in)
    :VertexBufferInterface(),
    vertices{ vertices_in },
    indices{ indices_in } {}

SimpleStaticMesh::SimpleStaticMesh(Verts vertices_in, Ints indices_in, Texs textures_in)
    :VertexBufferInterface(),
    vertices{ vertices_in },
    indices{ indices_in },
    textures{ textures_in } {}

// === Rendering ===
void SimpleStaticMesh::loadData() {
    bindAllBuffers();

    loadVerticesStatic(&vertices[0], sizeof(vertices) * sizeof(Vertex));
    loadIndicesStatic(&indices[0], sizeof(indices) * sizeof(unsigned int));

    configAttribf(0, 3, sizeof(Vertex), (void*)(0*sizeof(float)));
    configAttribf(1, 3, sizeof(Vertex), (void*)(3*sizeof(float)));
    configAttribf(2, 2, sizeof(Vertex), (void*)(6*sizeof(float)));

    unbindCurrent();
}

void SimpleStaticMesh::draw(ShaderProgram& program) {
    program.use();
    bindCurrent();
    
    drawElementsStatic(sizeof(indices));

    unbindCurrent();
}