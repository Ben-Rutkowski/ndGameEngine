#ifndef EDIT_FACE_HPP
#define EDIT_FACE_HPP

#include "em_sub_objects/edit_edge.hpp"
#include "em_sub_objects/edit_tri.hpp"

/* CLASS: EditMeshFace
A MeshFace is the over arching class for tris.
When constructing users can make a tri or quad.

It must be done through the EditMesh class.
When expanding the edges in you must add three
points, the first as the new point, and two edges.
*/

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

    int pointLen();
    int vertLen();
    int edgeLen();
    int triLen();

    Id getPointId(int i);
    Id getVertId(int i);
    Id getEdgeId(int i);
    Id getTriId(int i);

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
    Id createFace() {
        return createData();
    }
};

#endif