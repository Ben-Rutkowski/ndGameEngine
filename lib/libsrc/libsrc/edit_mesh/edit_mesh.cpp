#include "edit_mesh/edit_mesh.hpp"
#include "math/select_algs.hpp"


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

void EditMesh::extrudeTest(Id face) {
    float AMOUNT = 0.1f;

    int pointN  = face_cache[face].pointNum();
    vec4 normal = face_cache[face].calcNorm(tri_cache, vertex_cache);
    vec4 displace = normal.multK(AMOUNT, 3);

    // Create the new points
    std::vector<Id> new_points(pointN);
    Id point_id;
    vec4 point_pos;
    for (int i=0; i<pointN; i++) {
        point_id = face_cache[face].getPoint(i);
        point_pos = point_cache[point_id].getPos();
        point_pos = vec4::sumK(point_pos, displace, 3);
        new_points[i] = createPoint(point_pos);
    }
    std::vector<Id> new_edges(pointN);
    for (int i=0; i<pointN; i++) {
        new_edges[i] = createEdge({new_points[i%4], new_points[(i+1)%4]});
    }

    std::array<Id,4> points_quad {
        new_points[0], new_points[1], new_points[2], new_points[3]
    };
    std::array<Id,4> edges_quad = {
        new_edges[0], new_edges[1], new_edges[2], new_edges[3]
    };
    createQuad(points_quad, edges_quad);

    std::cout << "Point Number: " << point_cache.dataLen() << std::endl;

    load();
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
void EditMesh::selectFacesClick(vec4 point, vec4 camera_pos) {
    point = inverse_model_pos*point;
    vec4 u = inverse_model_pos*camera_pos;
    vec4 d = vec4::subtrK(point, u, 3);

    float curr_dist;
    float least_dist = -1.0f;
    int selected_face = -1;
    int N = face_cache.dataLen();
    for (int i=0; i<N; i++) {
        curr_dist = face_cache[i].rayIntersect(u, d, tri_cache, vertex_cache);
        if (curr_dist != -1.0f) {
            if (least_dist == -1.0f) {
                least_dist = curr_dist;
                selected_face = i;
            } else if ( curr_dist < least_dist ) {
                least_dist = curr_dist;
                selected_face = i;
            }
        }
    }
    
    bool select;
    for (int i=0; i<N; i++) {
        select = i == selected_face;
        selectFace(i, select);
    }
}

void EditMesh::selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos) {
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

void EditMesh::resetSelectFaces() {
    int N = face_cache.dataLen();
    select_faces.assign(N, false);
}

void EditMesh::selectPoint(Id id, bool value) {
    select_points[id] = value;
    point_cache[id].setSelect(value);
    updatePoint(id);
}

void EditMesh::selectFace(Id id, bool value) {
    select_faces[id] = value;
    int N = face_cache[id].vertNum();
    Id vert;
    for (int i=0; i<N; i++) {
        vert = face_cache[id].getVert(i);
        vertex_cache[vert].setSelect(value);
        updateVertex(vert);
    }
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