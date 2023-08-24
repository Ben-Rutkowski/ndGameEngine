#include "body.hpp"

// ======== BodyVertex ========
BodyVertex::BodyVertex(vec3 v)
    :vertex({v[0], v[1], v[2]}) {}
BodyVertex::BodyVertex(float arg0, float arg1, float arg2) 
    :vertex({arg0, arg1, arg2}) {}
vec4 BodyVertex::get() { return vertex; }
void BodyVertex::transform(mat4 trans) { vertex = trans*vertex; }


// ======== QUAD ========
// === Constructors ===
Quad::Quad(std::array<unsigned int, 4>& corner_in)
    :corners{ corner_in } {}
Quad::Quad(std::array<unsigned int, 4>&& corner_in)
    :corners{ corner_in } {}

// === Gets and Sets
unsigned int Quad::get(qCorner corn)          { return corners[(int)corn]; }
void Quad::set(qCorner corn, unsigned int id) { corners[(int)corn] = id; }

vec4 Quad::getEdge(qEdge edge, VCache& vertices) {
    vec4 tip  = vertices[tipID(edge)].get();
    vec4 tail = vertices[tailID(edge)].get();
    return tip - tail;
}

vec4 Quad::normalTri(qEdge e0, qEdge e1, VCache& vertices) {
    vec4 edge0 = getEdge(e0, vertices);
    vec4 edge1 = getEdge(e1, vertices);

    vec4 cross = edge0.cross(edge1);
    cross.normalize();
    cross.set(1.0f, 3);
    return cross;
}

vec4 Quad::normal(VCache& vertices) {
    vec4 norm_r = normalTri(qEdge::B, qEdge::R, vertices);
    vec4 norm_l = normalTri(qEdge::T, qEdge::L, vertices);

    vec4 norm = vec4::sumK(norm_r, norm_l, 3);
    norm.normalizeK(3);
    return norm;
}

// === Private ===
unsigned int Quad::tipID(qEdge edge) {
    int index = ((int)edge + 1) % 4;
    return corners[index];
}

unsigned int Quad::tailID(qEdge edge) {
    int index = (int)edge % 4;
    return corners[index];
}