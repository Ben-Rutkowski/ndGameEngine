#include "edit_mesh.hpp"

// === Constructors ===
EditMesh::EditMesh(VCache& vertices, QCache& quads)
    :v_cache{ vertices }, q_cache{ quads } {}

// === Rendering ===
void EditMesh::loadData() {

}

void EditMesh::draw(ShaderProgram& program) {

}

// === Static ===
int EditMesh::vCacheSize() {
    int n = v_cache.size();
    return n * sizeof(EditVertex);
}

int EditMesh::qCacheSize() {
    int n = q_cache.size();
    return n * sizeof(Quad);
}

EditMesh EditMesh::square() {
    VCache vertices = {
        EditVertex(-0.5f, -0.5f, 0.0f),
        EditVertex( 0.5f, -0.5f, 0.0f),
        EditVertex( 0.5f,  0.5f, 0.0f),
        EditVertex(-0.5f,  0.5f, 0.0f)
    };

    QCache quads = { Quad({0,1,2,3}) };

    return EditMesh(vertices, quads);
}