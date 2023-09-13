#include "edit_mesh/edit_face.hpp"
#include "math/select_algs.hpp"

void EditFace::addPoint(Id point_id)
    { points.push_back(point_id); }
void EditFace::addVertex(Id vert_id)
    { vertices.push_back(vert_id); }
void EditFace::addEdge(Id edge_id)
    { edges.push_back(edge_id); }
void EditFace::addTri(Id tri_id)
    { tris.push_back(tri_id); }

int EditFace::pointLen() { return points.size(); }
int EditFace::vertLen()  { return vertices.size(); }
int EditFace::edgeLen()  { return edges.size(); }
int EditFace::triLen()   { return tris.size(); }
Id EditFace::getPointId(int i) { return points[i]; }
Id EditFace::getVertId(int i)  { return vertices[i]; }
Id EditFace::getEdgeId(int i)  { return edges[i]; } 
Id EditFace::getTriId(int i)   { return tris[i]; } 

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
    int N = triLen();
    vec4 curr_norm;
    vec4 norm = tri_cache[tris[0]].calcNorm(vertex_cache);
    for (int i=1; i<N; i++) {
        curr_norm = tri_cache[tris[0]].calcNorm(vertex_cache);
        norm = vec4::aveUnits4f(norm, curr_norm);
    }
    return norm;
}

vec4 EditFace::calcCenter(PointCache& point_cache) {
    int N = pointLen();

    vec4 sum = vec4::basis(3);
    for (int i=0; i<N; i++) {
        vec4 current = point_cache[getPointId(i)].getPos();
        sum = vec4::sumK(sum, current, 3);
    }
    return sum.multK(1.0f/((float)N), 3);
}

void EditFace::setNorm(vec4 norm, VertexCache& vertex_cache) {
    int N = vertLen();
    for (int i=0; i<N; i++) {
        vertex_cache[vertices[i]].setNorm(norm);
    }
}

void EditFace::setCenter(vec4 center, VertexCache& vertex_cache) {
    int N = vertLen();
    for (int i=0; i<N; i++) {
        vertex_cache[vertices[i]].setCenter(center);
    }
}

float EditFace::rayIntersect(vec4 u, vec4 d, TriCache& tri_cache, VertexCache& vertex_cache) {
    vec4 v1, v2, v3;
    int N = triLen();
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