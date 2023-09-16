#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "rendering/vertex_buffer_interface.hpp"
#include "edit_mesh/edit_face.hpp"
#include "id_hash_table_old.hpp"

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

typedef IdHashTableOld IdSet;

class EditMesh {
private:
// Position
    mat4 model_pos;
    mat4 inverse_model_pos;

// Rendering
    VertexBufferInterface point_vbi;
    VertexBufferInterface line_vbi;
    VertexBufferInterface face_vbi;

// Caches
    PointCache  point_cache;
    VertexCache vertex_cache;
    EdgeCache   edge_cache;
    TriCache    tri_cache;
    FaceCache   face_cache;

// Other Attributes
    IdSet selected_points;
    IdSet selected_faces;

// --- Constructor ---
public:
    EditMesh();
    mat4 getModel();

private:
    EditPoint&    point(Id point_id);
    EditVertex&   vertex(Id vert_id);
    EdgeIndexObj& edge(Id edge_id);
    TriIndexObj&  tri(Id tri_id);
    EditFace&     face(Id face_id);

    Id vertToPoint(Id vert_id);
    Id pointToVert(Id point_id, int i);
    Id pointToEdge(Id point_id, int i);
    Id pointToFace(Id point_id, int i);
    Id edgeToFace(Id edge_id, int i);

// --- Model Transformations ---
public:
    void translate(vec4 trans);

// --- Editing ---
public:
    Id createPoint(vec4 pos);
    Id createEdge(Id2 point_ids);
    Id createTri(Id3 point_ids, Id3 edge_ids); // Points must be in CCW culling order
    Id createQuad(Id4 point_ids, Id4 edge_ids);    

    void transformPoints(IdSet& point_ids, mat4 mat);
    void transformPoints(mat4 mat);

private:
    void recalculateFaceCenter(Id face_id);
    void recalculateFaceNormal(Id face_id);
    // TODO: void recalculateFaceVerts(Id face_id);

    // --- Testing ---
public:
    void replaceTest(Id old_point_id, Id top_face_id);
    void replacePointDoesntWork(Id old_point_id, Id new_point_id, IdSet& faces_attached);
    void extrudeTest(Id face_id);

// --- Selecting ---
public:
    void selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos);
    void selectFaceClick(vec4 click, vec4 camera_pos);

    void clearSelectedPoints();
    void clearSelectedFaces();

private:
    void setSelectPoint(Id point_id, bool value);
    void setSelectFace(Id face_id, bool value);

    bool isPointSelect(Id point_id);
    bool checkCullPoint(Id id, vec4 camera_pos);    

// --- Debugging ---
public:
    vec4 getPoint(Id id);
    void printSelect();

// --- Rendering ---
public:
    void load();
    void drawPoints(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color);
    void drawLines(ShaderProgram& program, mat4 view, mat4 proj, vec4 color, vec4 select_color);
    void drawFaces(ShaderProgram& program, mat4 view, mat4 proj, vec4 camera_pos);

private:
    void reloadPoint(Id point_id);
    void reloadVertex(Id vert_id);
    void reloadFace(Id face_id);
};

#endif