/*

Point < Edge 
Vertex < Tri < Face

*/

#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "vertex_buffer_interface.hpp"
#include "edit_index_objects.hpp"

/* CLASS: EditMeshFace
A MeshFace is the over arching class for tris.
When constructing users can make a tri or quad.

It must be done through the EditMesh class.
When expanding the edges in you must add three
points, the first as the new point, and two edges.
*/

typedef unsigned int Id;
typedef std::array<unsigned int,2> Id2;
typedef std::array<unsigned int,3> Id3;
typedef std::array<unsigned int,4> Id4;

class EditFace {
private:
    std::vector<unsigned int> points;
    std::vector<unsigned int> vertices;
    std::vector<unsigned int> edges;
    std::vector<unsigned int> tris;

public:
    void addPoint(unsigned int point_id);
    void addVertex(unsigned int vert_id);
    void addEdge(unsigned int edge_id);
    void addTri(unsigned int tri_id);

    // unsigned int getPoint(int i, PointCache& cache);
    // EdgeIndexObj getEdge(int i, EdgeCache& cache);
    // TriIndexObj  getTri(int i, TriCache& tri_cache);

    // vec4 calcNorm(TriCache& tri_cache, VertexCache& vertex_cache);
};

/* CLASS FaceCache
Data : EditFace
*/

class FaceCache : public EditCache<EditFace> {
public:
    unsigned int createFace() {
        return createData();
    }
    
};

/* CLASS: EditMesh
======== ATTRIBUTES ========

======== METHODS ========
- addPoint : Adds a point to the point_cache
- addEdge : Adds an edge to the edge cache from existing points
    in the point_cache and updates the reference indices in the
    point_cache.

- addQuad : Creates a quad out of existing points.

When Creating a Quad or any face, you must create the points first,
then the edges, then the face

*/

class EditMesh {
// Rendering
    VertexBufferInterface vbi;

// Vertex Data
    PointCache  point_cache;
    VertexCache vertex_cache;

// Index Data
    EdgeCache edge_cache;
    TriCache  tri_cache;

// Face Data
    FaceCache face_cache;

public:
    Id createPoint(vec4 point);
    Id createEdge(Id2 points);
    Id createTri(Id3 points, Id3 edges);

    void load();
    void draw(ShaderProgram& program);
};

#endif