/*

Point < Edge 
Vertex < Tri < Face

*/

#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "vertex_buffer_interface.hpp"
#include "edit_index_objects.hpp"
#include "matrix.hpp"

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

    int pointNum();
    int vertNum();
    int edgeNum();
    int triNum();
    Id  getPoint(int i);
    Id  getVert(int i);
    Id  getEdge(int i);
    Id  getTri(int i);

    vec4 calcNorm(TriCache& tri_cache, VertexCache& vertex_cache);
    vec4 calcCenter(PointCache& point_cache);
    void setNorm(vec4 norm, VertexCache& vertex_cache);
    void setCenter(vec4 center, VertexCache& vertex_cache);
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
private:
// Position
    mat4 model_pos;

// Rendering
    VertexBufferInterface point_vbi;
    VertexBufferInterface line_vbi;
    VertexBufferInterface face_vbi;

// Vertex Data
    PointCache  point_cache;
    VertexCache vertex_cache;

// Index Data
    EdgeCache edge_cache;
    TriCache  tri_cache;

// Face Data
    FaceCache face_cache;

public:
    EditMesh();

// Transformation
public:
    void translate(vec4 trans);
    void setModelMat(mat4 model);

// Editing
public:
    Id createPoint(vec4 point);
    Id createEdge(Id2 points);
    Id createTri(Id3 points, Id3 edges);
    Id createQuad(Id4 points, Id4 edges);

// Rendering
public:
    void load();
    // void draw(ShaderProgram& points, ShaderProgram& lines, ShaderProgram& faces, mat4 view, mat4 proj, vec4 cfront);
    void drawPoints(ShaderProgram& program, mat4 view, mat4 proj, vec4 color);
    void drawLines(ShaderProgram& program, mat4 view, mat4 proj, vec4 color);
    void drawFaces(ShaderProgram& program, mat4 view, mat4 proj, vec4 camera_pos);
};

#endif