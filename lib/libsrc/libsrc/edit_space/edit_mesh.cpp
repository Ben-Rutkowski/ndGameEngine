#include "edit_space/edit_mesh.hpp"

void EditFace::addPoint(Id point_id)
    { points.push_back(point_id); }
void EditFace::addVertex(Id vert_id)
    { vertices.push_back(vert_id); }
void EditFace::addEdge(Id edge_id)
    { edges.push_back(edge_id); }
void EditFace::addTri(Id tri_id)
    { tris.push_back(tri_id); }

int EditFace::pointNum() { return points.size(); }
int EditFace::vertNum()  { return vertices.size(); }
int EditFace::edgeNum()  { return edges.size(); }
int EditFace::triNum()   { return tris.size(); }
Id EditFace::getPoint(int i) { return points[i]; }
Id EditFace::getVert(int i)  { return vertices[i]; }
Id EditFace::getEdge(int i)  { return edges[i]; } 
Id EditFace::getTri(int i)   { return tris[i]; } 

vec4 EditFace::edgeTipPos(Id edge, EdgeCache& edge_cache, PointCache& point_cache) {
    return edge_cache[edge].getTip(point_cache);
}

vec4 EditFace::edgeTailPos(Id edge, EdgeCache& edge_cache, PointCache& point_cache) {
    return edge_cache[edge].getTail(point_cache);
}

vec4 EditFace::triPos(Id tri, int i, TriCache& tri_cache, VertexCache& vertex_cache) {
    return tri_cache[tri].getPos(i, vertex_cache);
}

vec4 EditFace::calcNorm(TriCache& tri_cache, VertexCache& vertex_cache) {
    int N = triNum();
    vec4 curr_norm;
    vec4 norm = tri_cache[tris[0]].calcNorm(vertex_cache);
    for (int i=1; i<N; i++) {
        curr_norm = tri_cache[tris[0]].calcNorm(vertex_cache);
        norm = vec4::aveUnits4f(norm, curr_norm);
    }
    return norm;
}

vec4 EditFace::calcCenter(PointCache& point_cache) {
    int N = pointNum();

    vec4 sum = vec4::basis(3);
    for (int i=0; i<N; i++) {
        vec4 current = point_cache[getPoint(i)].getPos();
        sum = vec4::sumK(sum, current, 3);
    }
    return sum.multK(1.0f/((float)N), 3);
}

void EditFace::setNorm(vec4 norm, VertexCache& vertex_cache) {
    int N = vertNum();
    for (int i=0; i<N; i++) {
        vertex_cache[vertices[i]].setNorm(norm);
    }
}

void EditFace::setCenter(vec4 center, VertexCache& vertex_cache) {
    int N = vertNum();
    for (int i=0; i<N; i++) {
        vertex_cache[vertices[i]].setCenter(center);
    }
}

bool EditFace::isSelectPointClick(vec2 point, mat4 pvm, EdgeCache& edge_cache, PointCache& point_cache) {
    int cross_right = 0;
    int N = edgeNum();
    Id edge;
    vec4 root, end;
    for (int i=0; i<N; i++) {
        edge = edges[i];
        root = edgeTailPos(edge, edge_cache, point_cache);
        end  = edgeTipPos(edge, edge_cache, point_cache);
        root = sAlg::modelToClip(root, pvm);
        end  = sAlg::modelToClip(end, pvm);

        if (sAlg::crossRight(point, root, end)) {
            cross_right = cross_right + 1;
        }
    }

    if (cross_right%2 == 1) { return true; }
    else                    { return false; }
}

float EditFace::rayIntersect(vec4 u, vec4 d, TriCache& tri_cache, VertexCache& vertex_cache) {
    vec4 v1, v2, v3;
    int N = triNum();
    float dist = -1.0f;
    float curr_dist = -1.0f;
    for (int i=0; i<N; i++) {
        v1 = triPos(tris[i], 0, tri_cache, vertex_cache);
        v2 = triPos(tris[i], 1, tri_cache, vertex_cache);
        v3 = triPos(tris[i], 2, tri_cache, vertex_cache);
        curr_dist = sAlg::triRayIntersect(v1, v2, v3, u, d);
        if (curr_dist != -1.0f) {
            dist = curr_dist;
        } 
    }
    return dist;
}

// ======== EditMesh ========
EditMesh::EditMesh()
    :model_pos{ mat4::iden() },
    inverse_model_pos{ mat4::iden() } {} 

mat4 EditMesh::getModel() {
    return model_pos;
}

void EditMesh::translate(vec4 trans) {
    model_pos = mat4::translate(trans)*model_pos;
    mat4 inverse = mat4::itranslate(trans);
    inverse_model_pos = inverse_model_pos*inverse;
}

Id EditMesh::createPoint(vec4 point)
    { return point_cache.addPoint(point); }

unsigned int EditMesh::createEdge(Id2 points) {
    Id new_edge = edge_cache.addEdge(EdgeIndexObj(points[0], points[1]));
    point_cache.pairEdge(points[0], new_edge);
    point_cache.pairEdge(points[1], new_edge);
    return new_edge;
}

Id EditMesh::createTri(Id3 points, Id3 edges) {
    Id3  tri_verts; 
    vec4 curr_pos;
    
    Id new_face_id = face_cache.createFace();
    EditFace& new_face = face_cache[new_face_id];

    for (int i=0; i<3; i++) {
        point_cache.pairFace(points[i], new_face_id);
        new_face.addPoint(points[i]);

        curr_pos = point_cache[points[i]].getPos();
        Id vert = vertex_cache.addVertex(curr_pos);
        point_cache.pairVertex(points[i], vert);
        new_face.addVertex(vert);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri(tri_verts[0], tri_verts[1], tri_verts[2]);
    Id new_tri_id = tri_cache.addTri(tri);
    new_face.addTri(new_tri_id);

    vec4 norm = new_face.calcNorm(tri_cache, vertex_cache);
    vec4 center = new_face.calcCenter(point_cache);
    new_face.setNorm(norm, vertex_cache);
    new_face.setCenter(center, vertex_cache);

    return new_face_id;
} 

Id EditMesh::createQuad(Id4 points, Id4 edges) {
    Id4 tri_verts;
    vec4 curr_pos;

    Id new_face_id = face_cache.createFace();
    EditFace& new_face = face_cache[new_face_id];

    for(int i=0; i<4; i++) {
        point_cache.pairFace(points[i], new_face_id);
        new_face.addPoint(points[i]);

        curr_pos = point_cache[points[i]].getPos();
        Id vert = vertex_cache.addVertex(curr_pos);
        point_cache.pairVertex(points[i], vert);
        new_face.addVertex(vert);
        tri_verts[i] = vert;

        edge_cache.pairFace(edges[i], new_face_id);
        new_face.addEdge(edges[i]);
    }

    TriIndexObj tri1(tri_verts[0], tri_verts[1], tri_verts[2]);
    TriIndexObj tri2(tri_verts[2], tri_verts[3], tri_verts[0]);
    Id new_tri_id1 = tri_cache.addTri(tri1);
    Id new_tri_id2 = tri_cache.addTri(tri2);
    new_face.addTri(new_tri_id1);
    new_face.addTri(new_tri_id2);

    vec4 norm = new_face.calcNorm(tri_cache, vertex_cache);
    vec4 center = new_face.calcCenter(point_cache);
    new_face.setNorm(norm, vertex_cache);
    new_face.setCenter(center, vertex_cache);

    return new_face_id;
}

void EditMesh::setPointPos(Id point, vec4 pos) {
    point_cache[point].setPos(pos);
    reloadPoint(point);

    Id vert_id;
    int N = point_cache.vertexNum(point);
    for (int i=0; i<N; i++) {
        vert_id = point_cache.getVertex(point, i);
        vertex_cache[vert_id].setPos(pos);
    }

    Id face_id;
    N = point_cache.faceNum(point);
    for (int i=0; i<N; i++) {
        face_id = point_cache.getFace(point, i);
        face_cache[face_id].calcNorm(tri_cache, vertex_cache);
        face_cache[face_id].calcCenter(point_cache);
        reloadFace(face_id);
    }
}

void EditMesh::translatePoint(Id point, vec4 trans) {
    vec4 point_pos = point_cache[point].getPos();
    vec4 pos = vec4::sumK(point_pos, trans, 3);
    setPointPos(point, pos);
}

void EditMesh::translateSelectPoints(vec4 trans) {
    int N = select_points.size();
    for (int i=0; i<N; i++) {
        if (select_points[i]) {
            translatePoint(i, trans);
        }
    }
}

void EditMesh::reloadPoint(Id point) {
    point_vbi.bindAllBuffers();
    point_vbi.editVertexData(&point_cache[point], sizeof(EditPoint), point*sizeof(EditPoint));
    point_vbi.unbindCurrent();
    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&point_cache[point], sizeof(EditPoint), point*sizeof(EditPoint));
    line_vbi.unbindCurrent();
}

void EditMesh::reloadFace(Id face) {
    Id vert_id;
    int N = face_cache[face].vertNum();
    face_vbi.bindAllBuffers();
    for (int i=0; i<N; i++) {
        vert_id = face_cache[face].getVert(i);
        face_vbi.editVertexData(&vertex_cache[vert_id], sizeof(EditVertex), vert_id*sizeof(EditVertex));
    }
    face_vbi.unbindCurrent();
}

// === Debugging ===
vec4 EditMesh::getPoint(Id id) {
    return point_cache[id].getPos();
}

void EditMesh::printSelect() {
    int N = point_cache.dataLen();
    for (int i=0; i<N; i++) {
        if (select_points[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

// === Selecting ===
void EditMesh::selectFaces(vec2 click, mat4 pvm) {
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        if (face_cache[i].isSelectPointClick(click, pvm, edge_cache, point_cache)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

void EditMesh::selectPointBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos) {
    v1 = inverse_model_pos*v1;
    v2 = inverse_model_pos*v2;
    v3 = inverse_model_pos*v3;
    camera_pos = inverse_model_pos*camera_pos;
    vec4 u = camera_pos;
    vec4 d;
    vec4 point;
    float dist;
    bool selected;

    int N = point_cache.dataLen();
    for (int i=0; i<N; i++) {
        point = point_cache[i].getPos();
        d     = vec4::subtrK(point, camera_pos, 3);
        dist  = sAlg::parRayIntersect(v1, v2, v3, u, d);
        selected = dist > 0.0f;
        selectPoint(i, selected);
        if ( selected ) {
            cullPoint(i, camera_pos);
        }
    }
}

// === Private ===
void EditMesh::resetSelectPoints() {
    int N = point_cache.dataLen();
    select_points.assign(N, false);
}

void EditMesh::selectPoint(Id id, bool value) {
    select_points[id] = value;
    point_cache[id].setSelect(value);
    updatePoint(id);
}

void EditMesh::cullPoint(Id id, vec4 camera_pos) {
    float dist;
    vec4 u = point_cache[id].getPos();
    vec4 d = vec4::subtrK(camera_pos, u, 3);
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        dist = face_cache[i].rayIntersect(u, d, tri_cache, vertex_cache);
        if ( dist != -1.0f) {
            selectPoint(id, false);
            return;
        }
    }
}

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
    face_vbi.unbindCurrent();

    resetSelectPoints();
}

void EditMesh::updatePoint(Id id) {
    point_vbi.bindAllBuffers();
    point_vbi.editVertexData(&point_cache[id], sizeof(EditPoint), id*sizeof(EditPoint));
    point_vbi.unbindCurrent();

    line_vbi.bindAllBuffers();
    line_vbi.editVertexData(&point_cache[id], sizeof(EditPoint), id*sizeof(EditPoint));
    line_vbi.unbindCurrent();
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