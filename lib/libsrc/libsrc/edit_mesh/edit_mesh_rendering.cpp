#include "edit_mesh/edit_mesh.hpp"

// === Rendering ===
void EditMesh::load() {
    point_vbi.bindAllBuffers();
    point_vbi.loadVerticesStream(point_cache.dataPtr(), point_cache.dataSize());
    point_vbi.configAttribf(0, 4, sizeof(EditPoint), (void*)0);
    point_vbi.configAttribf(1, 1, sizeof(EditPoint), (void*)sizeof(vec4));
    point_vbi.unbindCurrent();

    line_vbi.bindAllBuffers();
    line_vbi.loadVerticesStream(point_cache.dataPtr(), point_cache.dataSize());
    line_vbi.loadIndicesStatic(edge_cache.dataPtr(), edge_cache.dataSize());
    line_vbi.configAttribf(0, 4, sizeof(EditPoint), (void*)0);
    line_vbi.configAttribf(1, 1, sizeof(EditPoint), (void*)sizeof(vec4));
    line_vbi.unbindCurrent();

    face_vbi.bindAllBuffers();
    face_vbi.loadVerticesStream(vertex_cache.dataPtr(), vertex_cache.dataSize());
    face_vbi.loadIndicesStatic(tri_cache.dataPtr(), tri_cache.dataSize());
    face_vbi.configAttribf(0, 4, sizeof(EditVertex), (void*)0);
    face_vbi.configAttribf(1, 4, sizeof(EditVertex), (void*)(sizeof(vec4)));
    face_vbi.configAttribf(2, 4, sizeof(EditVertex), (void*)(2*sizeof(vec4)));
    face_vbi.configAttribf(3, 1, sizeof(EditVertex), (void*)(3*sizeof(vec4) + sizeof(vec2)));
    face_vbi.unbindCurrent();

    resetSelectPoints();
    resetSelectFaces();
}

void EditMesh::updatePoint(Id id) {
    point_vbi.bindAllBuffers();
    point_vbi.editVertexData(&point_cache[id], sizeof(EditPoint), id*sizeof(EditPoint));
    point_vbi.unbindCurrent();

    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&point_cache[id], sizeof(EditPoint), id*sizeof(EditPoint));
    line_vbi.unbindCurrent();
}

void EditMesh::updateVertex(Id id) {
    face_vbi.bindAllBuffers();
    face_vbi.editVertexData(&vertex_cache[id], sizeof(EditVertex), id*sizeof(EditVertex));
    face_vbi.unbindCurrent();
}

void EditMesh::drawPoints(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    program.uniform4f("color", color);
    program.uniform4f("select_color", select_color);

    point_vbi.bindCurrent();
    point_vbi.drawPoints(point_cache.dataLen());
    point_vbi.unbindCurrent();
}

void EditMesh::drawLines(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    program.uniform4f("color", color);
    program.uniform4f("select_color", select_color);

    line_vbi.bindCurrent();
    line_vbi.drawElementsLines(edge_cache.indexLen());
    line_vbi.unbindCurrent();
    glDepthMask(GL_TRUE);
}

void EditMesh::drawFaces(ShaderProgram& program, mat4 view, mat4 proj, vec4 camera_pos) {
    program.use();
    program.uniformMat4f("model", model_pos);
    program.uniformMat4f("view", view);
    program.uniformMat4f("proj", proj);
    program.uniform4f("camera_pos", camera_pos);

    face_vbi.bindCurrent();
    face_vbi.drawElementsTriangles(tri_cache.indexLen());
    face_vbi.unbindCurrent();
}