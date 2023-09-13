/*

Point < Edge 
Vertex < Tri < Face

*/

#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "rendering/vertex_buffer_interface.hpp"
#include "edit_space/edit_index_objects.hpp"
#include "math/select_algs.hpp"

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
    std::vector<Id> points;
    std::vector<Id> vertices;
    std::vector<Id> edges;
    std::vector<Id> tris;

public:
    void addPoint(Id point_id);
    void addVertex(Id vert_id);
    void addEdge(Id edge_id);
    void addTri(Id tri_id);

    int pointNum();
    int vertNum();
    int edgeNum();
    int triNum();

    Id getPoint(int i);
    Id getVert(int i);
    Id getEdge(int i);
    Id getTri(int i);

    vec4 edgeTipPos(Id edge, EdgeCache& edge_cache, PointCache& point_cache);
    vec4 edgeTailPos(Id edge, EdgeCache& edge_cache, PointCache& point_cache);
    vec4 triPos(Id tri, int i, TriCache& tri_cache, VertexCache& vertex_cache);

    vec4 calcNorm(TriCache& tri_cache, VertexCache& vertex_cache);
    vec4 calcCenter(PointCache& point_cache);
    void setNorm(vec4 norm, VertexCache& vertex_cache);
    void setCenter(vec4 center, VertexCache& vertex_cache);

    float rayIntersect(vec4 u, vec4 d, TriCache& tri_cache, VertexCache& vertex_cache); // Returns the length of the ray that it hits the face, if it intersects, else -1.0f
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
    mat4 inverse_model_pos;

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

// Other Attributes
    std::vector<bool> select_points;
    std::vector<bool> select_faces;

public:
    EditMesh();
    mat4 getModel();

// Transformation
public:
    void translate(vec4 trans);

// Editing
public:
    Id createPoint(vec4 point);
    Id createEdge(Id2 points);
    Id createTri(Id3 points, Id3 edges);
    Id createQuad(Id4 points, Id4 edges);

    void setPointPos(Id point, vec4 pos);
    void translatePoint(Id point, vec4 trans);
    void translateSelectPoints(vec4 trans);

private:
    void reloadPoint(Id point);
    void reloadFace(Id face);

// Selecting
public:
    void selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos);
    void selectFacesClick(vec4 point, vec4 camera_pos);

private:
    void resetSelectPoints();
    void resetSelectFaces();
    void selectPoint(Id id, bool value);
    void selectFace(Id id, bool value);
    void cullPoint(Id id, vec4 camera_pos); // Sets point to false select if it is behind a face.

// Debugging
public:
    vec4 getPoint(Id id);
    void printSelect();

// Rendering
public:
    void load();
    void updatePoint(Id id);
    void updateVertex(Id id);
    void drawPoints(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color);
    void drawLines(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color);
    void drawFaces(ShaderProgram& program, mat4 view, mat4 proj, vec4 camera_pos);
};

#endif