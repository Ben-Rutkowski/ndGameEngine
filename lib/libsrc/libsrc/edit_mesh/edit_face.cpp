#include "edit_mesh/edit_face.hpp"
#include "math/select_algs.hpp"

EditFace::EditFace(int n)
    :N_sides{n},
    point_ids_new(n),
    vert_ids_new(n),
    edge_ids_new(n),
    tri_ids_new(n-2) {}

// void EditFace::addPoint(Id point_id) { point_ids.push_back(point_id); }
// void EditFace::addEdge(Id edge_id)   { edge_ids.push_back(edge_id); }
// void EditFace::addVertex(Id vert_id) { vert_ids.push_back(vert_id); }
// void EditFace::addTri(Id tri_id)     { tri_ids.push_back(tri_id); }

// int EditFace::pointLen() { return vert_ids.size(); }
// int EditFace::edgeLen()  { return edge_ids.size(); }
// int EditFace::vertLen()  { return vert_ids.size(); }
// int EditFace::triLen()   { return tri_ids.size(); }

// Id EditFace::pointId(int i) { return point_ids[i]; }
// Id EditFace::edgeId(int i)  { return edge_ids[i]; } 
// Id EditFace::vertId(int i)  { return vert_ids[i]; }
// Id EditFace::triId(int i)   { return tri_ids[i]; } 

// EditPoint&  EditFace::point(int i, PointCache& pc) { return pc[point_ids[i]]; }
// EdgeIndexObj& EditFace::edge(int i, EdgeCache& ec) { return ec[edge_ids[i]]; }
// EditVertex& EditFace::vert(int i, VertexCache& vc) { return vc[vert_ids[i]]; }
// TriIndexObj&  EditFace::tri(int i, TriCache& tc)   { return tc[tri_ids[i]]; }


// void EditFace::addPoint(Id point_id) { point_ids_new.add(point_id); }
void EditFace::addPoint(Id point_id, Id vert_id) { point_ids_new.add(point_id, vert_id); }
void EditFace::addEdge(Id edge_id)   { edge_ids_new.add(edge_id); }
void EditFace::addVertex(Id vert_id) { vert_ids_new.add(vert_id); }
void EditFace::addTri(Id tri_id)     { tri_ids_new.add(tri_id); }

int EditFace::pointLen() { return N_sides; }
int EditFace::edgeLen()  { return N_sides; }
int EditFace::vertLen()  { return N_sides; }
int EditFace::triLen()   { return N_sides - 2; }

Id EditFace::pointId(int i) { return point_ids_new.forceGetKey(i); }
Id EditFace::edgeId(int i)  { return edge_ids_new.forceGetKey(i); } 
Id EditFace::vertId(int i)  { return vert_ids_new.forceGetKey(i); }
Id EditFace::triId(int i)   { return tri_ids_new.forceGetKey(i); } 

EditPoint&  EditFace::point(int i, PointCache& pc) { return pc[point_ids_new.forceGetKey(i)]; }
EdgeIndexObj& EditFace::edge(int i, EdgeCache& ec) { return ec[edge_ids_new.forceGetKey(i)]; }
EditVertex& EditFace::vert(int i, VertexCache& vc) { return vc[vert_ids_new.forceGetKey(i)]; }
TriIndexObj&  EditFace::tri(int i, TriCache& tc)   { return tc[tri_ids_new.forceGetKey(i)]; }

bool EditFace::hasPoint(Id point_id) { return point_ids_new.hasElement(point_id); }
bool EditFace::hasEdge(Id edge_id)   { return edge_ids_new.hasElement(edge_id); }

// === Replacing ===
bool EditFace::replacePoint(Id old_point_id, Id new_point_id, PointCache& pc, VertexCache& vc) {
    bool contains;
    Id vert_id = point_ids_new.value(old_point_id);
    contains   = point_ids_new.remove(old_point_id);
    if (contains) { 
        point_ids_new.add(new_point_id, vert_id);
        vec4 pos = pc[new_point_id].getPos();
        vc[vert_id].setPos(pos);
        vc.pairPoint(vert_id, new_point_id);
        pc.removeVertex(old_point_id, vert_id);
        pc.pairVertex(new_point_id, vert_id);
        return true;
    } else {
        return false;
    }
}   

bool EditFace::replaceEdge(Id old_edge_id, Id new_edge_id) {
    bool contains = edge_ids_new.remove(old_edge_id);
    if (contains) {
        edge_ids_new.add(new_edge_id);
        return true;
    } else {
        return false;
    }
}

// === Calculations ===
vec4 EditFace::calcNorm(TriCache& tc, VertexCache& vc) {
    // int N = triLen();
    // vec4 curr_norm;
    // vec4 norm = tri_cache[tri_ids[0]].calcNorm(vertex_cache);
    // for (int i=1; i<N; i++) {
    //     curr_norm = tri_cache[tri_ids[0]].calcNorm(vertex_cache);
    //     norm = vec4::aveUnits4f(norm, curr_norm);
    // }
    // return norm;

    vec4 curr_norm;
    vec4 norm = tri(0, tc).calcNorm(vc);
    // vec4 norm = tc[tre]
    int N_tri = triLen();
    for (int i=1; i<N_tri; i++) {
        curr_norm = tri(i, tc).calcNorm(vc);
        norm = vec4::aveUnits4f(norm, curr_norm);
    }
    return norm;
}

vec4 EditFace::calcCenter(PointCache& pc) {
    // int N = pointLen();

    // vec4 sum = vec4::basis(3);
    // for (int i=0; i<N; i++) {
    //     vec4 current = point_cache[pointId(i)].getPos();
    //     sum = vec4::sumK(sum, current, 3);
    // }
    // return sum.multK(1.0f/((float)N), 3);

    vec4 curr_center;
    int N_point = pointLen();
    vec4 sum = vec4::basis(3);
    for (int i=0; i<N_point; i++) {
        curr_center = point(i, pc).getPos();
        sum = vec4::sumK(sum, curr_center, 3);
    }
    return sum.multK(1.0f/((float)N_point), 3);
    
}

void EditFace::setNorm(vec4 norm, VertexCache& vc) {
    // int N = vertLen();
    // for (int i=0; i<N; i++) {
    //     vertex_cache[vert_ids[i]].setNorm(norm);
    // }

    int N_vert = vertLen();
    for (int i=0; i<N_vert; i++) {
        vert(i, vc).setNorm(norm);
    }
}

void EditFace::setCenter(vec4 center, VertexCache& vc) {
    // int N = vertLen();
    // for (int i=0; i<N; i++) {
    //     vertex_cache[vert_ids[i]].setCenter(center);
    // }

    int N_vert = vertLen();
    for (int i=0; i<N_vert; i++) {
        vert(i, vc).setCenter(center);
    }
}

float EditFace::rayIntersect(vec4 u, vec4 d, TriCache& tc, VertexCache& vc) {
    // vec4 v1, v2, v3;
    // int N = triLen();
    // float dist = -1.0f;
    // float curr_dist = -1.0f;
    // for (int i=0; i<N; i++) {
    //     v1 = triPos(tri_ids[i], 0, tri_cache, vertex_cache);
    //     v2 = triPos(tri_ids[i], 1, tri_cache, vertex_cache);
    //     v3 = triPos(tri_ids[i], 2, tri_cache, vertex_cache);
    //     curr_dist = sAlg::triRayIntersect(v1, v2, v3, u, d);
    //     if (curr_dist != -1.0f) {
    //         dist = curr_dist;
    //     } 
    // }
    // return dist;

    vec4 v1, v2, v3;
    bool intersect  = false;
    float dist      = -1.0f;
    float curr_dist = 1.0f;
    int N_tri = triLen();
    for (int i=0; i<N_tri; i++) {
        v1 = tri(i, tc).vert(0, vc).getPos();
        v2 = tri(i, tc).vert(1, vc).getPos();
        v3 = tri(i, tc).vert(2, vc).getPos();
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

    point_ids_new.printData();
    point_ids_new.printOpen();

    std::cout << "Verts: " << std::endl;

    vert_ids_new.printData();
    vert_ids_new.printOpen();

    std::cout << "Edges: " << std::endl;

    edge_ids_new.printData();
    edge_ids_new.printOpen();

    std::cout << "Tris: " << std::endl;

    tri_ids_new.printData();
    tri_ids_new.printOpen();
}

void EditFace::printSmall() {
    std::cout << std::endl;
    std::cout << "Points: " << std::endl;

    point_ids_new.printDataSmall();

    std::cout << "Verts: " << std::endl;

    vert_ids_new.printDataSmall();

    std::cout << "Edges: " << std::endl;

    edge_ids_new.printDataSmall();

    std::cout << "Tris: " << std::endl;

    tri_ids_new.printDataSmall();
}

void EditFace::debug(VertexCache& vc) {
    print();
}