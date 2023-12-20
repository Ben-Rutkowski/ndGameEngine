/*

A simple static mesh is a game object whose vertices have 
vec3 position coordinates, vec3 normal coordinates, and
vec2 texture coordinates. The mesh also has elements indices.

*/

#include  "vertex_buffer_interface.hpp"

#ifndef SIMPLE_STATIC_MESH_HPP
#define SIMPLE_STATIC_MESH_HPP

#include <vector>

typedef std::vector<Vertex>       Verts;
typedef std::vector<unsigned int> Ints;
typedef std::vector<Texture>      Texs;

class SimpleStaticMesh : public VertexBufferInterface {
// --- Attributes ---
private:
    Verts vertices;
    Ints  indices;
    Texs  textures;

// --- Constructors and Initialization ---
public:
    SimpleStaticMesh();
    SimpleStaticMesh(Verts vertices_in, Ints indices_in);
    SimpleStaticMesh(Verts vertices_in, Ints indices_in, Texs textures_in);

// --- Rendering ---
public:
    void loadData();
    void draw(ShaderProgram& program);
};

#endif