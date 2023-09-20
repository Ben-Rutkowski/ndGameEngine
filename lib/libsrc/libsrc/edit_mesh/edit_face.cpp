#include "edit_mesh/edit_face.hpp"
#include "math/select_algs.hpp"

EditFace::EditFace(int n)
    :N_sides{n},
    point_id_cache(n),
    edge_id_cache(n),
    tri_id_cache(n-2) {}

void EditFace::addPoint(Id point_id, Id vert_id) { point_id_cache.add(point_id, vert_id); }
void EditFace::addEdge(Id edge_id)               { edge_id_cache.add(edge_id); }
void EditFace::addTri(Id tri_id)                 { tri_id_cache.add(tri_id); }

int EditFace::pointLen() { return N_sides; }
int EditFace::edgeLen()  { return N_sides; }
int EditFace::triLen()   { return N_sides - 2; }

Id EditFace::pointId(int i) { return point_id_cache.forceGetKey(i); }
Id EditFace::vertId(int i)  { return point_id_cache.forceGetValue(i); }
Id EditFace::edgeId(int i)  { return edge_id_cache.forceGetKey(i); } 
Id EditFace::triId(int i)   { return tri_id_cache.forceGetKey(i); } 

bool EditFace::hasPoint(Id point_id) { return point_id_cache.hasElement(point_id); }
bool EditFace::hasEdge(Id edge_id)   { return edge_id_cache.hasElement(edge_id); }

// === Replacing ===
bool EditFace::replacePoint(Id old_point_id, Id new_point_id, PointCache& pc, VertexCache& vc) {
    bool contains;
    Id vert_id = point_id_cache.value(old_point_id);
    contains   = point_id_cache.remove(old_point_id);
    if (contains) { 
        point_id_cache.add(new_point_id, vert_id);
        return true;
    } else {
        return false;
    }
}   

bool EditFace::replaceEdge(Id old_edge_id, Id new_edge_id) {
    bool contains = edge_id_cache.remove(old_edge_id);
    if (contains) {
        edge_id_cache.add(new_edge_id);
        return true;
    } else {
        return false;
    }
}

// === Calculations ===
vec4 EditFace::calcNorm(TriCache& tc, VertexCache& vc) {
    vec4 curr_norm;
    // vec4 norm = tri(0, tc).calcNorm(vc);
    vec4 norm = tc[triId(0)].calcNorm(vc);
    int N_tri = triLen();
    for (int i=1; i<N_tri; i++) {
        // curr_norm = tri(i, tc).calcNorm(vc);
        curr_norm = tc[triId(i)].calcNorm(vc);
        norm = vec4::aveUnits4f(norm, curr_norm);
    }
    return norm;
}

vec4 EditFace::calcCenter(PointCache& pc) {
    vec4 curr_center;
    int N_point = pointLen();
    vec4 sum = vec4::basis(3);
    for (int i=0; i<N_point; i++) {
        curr_center = pc[pointId(i)].getPos();
        sum = vec4::sumK(sum, curr_center, 3);
    }
    return sum.multK(1.0f/((float)N_point), 3);
}

vec4 EditFace::calcVertPos(Id point_id, PointCache& pc) {
    return pc[point_id].getPos();
}

void EditFace::setNorm(vec4 norm, VertexCache& vc) {
    int N_vert = pointLen();
    for (int i=0; i<N_vert; i++) {
        vc[vertId(i)].setNorm(norm);
    }
}

void EditFace::setCenter(vec4 center, VertexCache& vc) {
    int N_vert = pointLen();
    for (int i=0; i<N_vert; i++) {
        vc[vertId(i)].setCenter(center);
    }
}

void EditFace::setPos(Id point_id, vec4 pos, VertexCache& vc) {
    Id vert_id = point_id_cache.value(point_id);
    vc[vert_id].setPos(pos);
}

float EditFace::rayIntersect(vec4 u, vec4 d, TriCache& tc, VertexCache& vc) {
    vec4 v1, v2, v3;
    bool intersect  = false;
    float dist      = -1.0f;
    float curr_dist = 1.0f;
    int N_tri = triLen();
    for (int i=0; i<N_tri; i++) {
        v1 = vc[tc[triId(i)].vertId(0)].getPos();
        v2 = vc[tc[triId(i)].vertId(1)].getPos();
        v3 = vc[tc[triId(i)].vertId(2)].getPos();
        curr_dist = sAlg::triRayIntersect(v1, v2, v3, u, d);
        if (curr_dist != -1.0f) {
            if (!intersect) {
                dist = curr_dist;
            } else if ( curr_dist < dist ) {
                dist = curr_dist;
            }
        }
    }
    return dist;
}

// === Debugging ===
void EditFace::print() {

    std::cout << std::endl << std::endl << std::endl;

    std::cout << "Points: " << std::endl;
    point_id_cache.printData();
    point_id_cache.printOpen();

    std::cout << "Edges: " << std::endl;
    edge_id_cache.printData();
    edge_id_cache.printOpen();

    std::cout << "Tris: " << std::endl;
    tri_id_cache.printData();
    tri_id_cache.printOpen();
}

void EditFace::printSmall() {
    std::cout << std::endl;
    std::cout << "Points: " << std::endl;
    point_id_cache.printDataSmall();

    std::cout << "Edges: " << std::endl;
    edge_id_cache.printDataSmall();

    std::cout << "Tris: " << std::endl;
    tri_id_cache.printDataSmall();
}

void EditFace::debug(VertexCache& vc) {
    print();
}