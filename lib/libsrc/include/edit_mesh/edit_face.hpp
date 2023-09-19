#ifndef EDIT_FACE_HPP
#define EDIT_FACE_HPP

#include "em_sub_objects/edit_edge.hpp"
#include "em_sub_objects/edit_tri.hpp"
#include "id_hash_table.hpp"

/* CLASS: EditMeshFace
A MeshFace is the over arching class for tris.
When constructing users can make a tri or quad.

It must be done through the EditMesh class.
When expanding the edges in you must add three
points, the first as the new point, and two edges.
*/

class EditFace {
private:
    int N_sides;

    // std::vector<Id> point_ids;
    // std::vector<Id> vert_ids;
    // std::vector<Id> edge_ids;
    // std::vector<Id> tri_ids;

    IdHashTableDynamic point_ids_new; // Point -> Vert
    IdHashTableDynamic vert_ids_new;  // Vert
    IdHashTableDynamic edge_ids_new;  // Edge  -> Point
    IdHashTableDynamic tri_ids_new;   // Tri   -> Vert

public:
    EditFace(int n);

    // void addPoint(Id point_id);
    void addPoint(Id point_id, Id vert_id);
    void addVertex(Id vert_id);
    void addEdge(Id edge_id);
    void addTri(Id tri_id);

    int pointLen();
    int vertLen();
    int edgeLen();
    int triLen();

    Id pointId(int i);
    Id vertId(int i);
    Id edgeId(int i);
    Id triId(int i);

    EditPoint&  point(int i, PointCache& pc);
    EditVertex& vert(int i, VertexCache& vc);
    EdgeIndexObj& edge(int i, EdgeCache& ec);
    TriIndexObj&  tri(int i, TriCache& tc);

    void setNorm(vec4 norm, VertexCache& vc);
    void setCenter(vec4 center, VertexCache& vc);

    bool hasPoint(Id point_id);
    bool hasEdge(Id edge_id);

// --- Replacing ---
    // Returns true if the point or edge is in the face
    bool replacePoint(Id old_point_id, Id new_point_id, PointCache& pc, VertexCache& vc);
    bool replaceEdge(Id old_edge_id, Id new_edge_ids);

// --- Calculations ---
    vec4  calcNorm(TriCache& tri_cache, VertexCache& vertex_cache);
    vec4  calcCenter(PointCache& point_cache);
    float rayIntersect(vec4 u, vec4 d, TriCache& tc, VertexCache& vc); // Returns the length of the ray that it hits the face, if it intersects, else -1.0f

// --- Debugging ---
public:
    void print();
    void printSmall();
    void debug(VertexCache& vc);
};

/* CLASS FaceCache
Data : EditFace
*/

class FaceCache : public EditCache<EditFace> {
public:
    // Id createFace() {
    //     return createData();
    // }

    Id createFace(int n) {
        return addData(EditFace(n));
    }
};

#endif