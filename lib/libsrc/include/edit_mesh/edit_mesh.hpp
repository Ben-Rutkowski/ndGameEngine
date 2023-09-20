/*  (-) Arbitrary Paired, (+) One Paired

    Point     Edge      Face (n)
    -----     ----      ----
    - Edge    - Face    n   Point -> Vert ( Vertices are unique to a face )
    - Face              n   Edge
                        n-2 Tri           ( Tris are in CCW winding order )
              
*/                                                   

#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

#include "rendering/vertex_buffer_interface.hpp"
#include "edit_mesh/edit_face.hpp"
#include "id_hash_table.hpp"

enum emEdgeType { null = 0, emSEEM, emTOP, emBOTTOM };

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

// typedef IdHashTableOld IdSet;
typedef IdHashTableStatic IdSet;

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
    Id createTri(Id3 point_ids, Id3 edge_ids); // Points must be in CCW winding order
    Id createQuad(Id4 point_ids, Id4 edge_ids);    
    Id createQuadFromEdge(Id point_id_0, Id point_id_1, IdSet& point_id_pairs, IdSet& edge_id_pairs); // Points 1 and 2 are the first in four in the CCW winding order
    Id duplicateEdge(Id old_edge_id, IdSet& point_id_pairs);


    void transformPoints(IdSet& point_ids, mat4 mat);
    void transformPoints(mat4 mat);

    void ripPoints(IdSet& old_point_ids, IdSet& edge_pairs_empty, IdSet& faces_attatched);

    // void transformPointsOld(IdSet& point_ids, mat4 mat);

// --- Test Features ---
    void extrudeFaceTest(Id face_id);

private:
    void recalculateFaceCenter(Id face_id);
    void recalculateFaceNormal(Id face_id);
    void recalculateFacePos(Id face_id, IdSet& point_ids);
    
    void replacePointInFaces(Id old_point_id, Id new_point_id, IdSet& faces_attatched, bool invert);
    void replacePointsInEdge(IdSet& point_id_pairs, Id edge_id);
    void replaceEdgeInFaces(Id old_edge_id, Id new_edge_id, IdSet& faces_attatched, bool invert);
    int  classifyEdges(IdSet& edges, IdSet& faces_attached);

// --- Selecting ---
public:
    void selectPointsBox(vec4 v1, vec4 v2, vec4 v3, vec4 camera_pos);
    void selectFaceClick(vec4 click, vec4 camera_pos);

    void clearSelectedPoints();
    void clearSelectedFaces();

    Id findEdge(Id point_id_0, Id point_id_1);

private:
    void setSelectPoint(Id point_id, bool value);
    void setSelectFace(Id face_id, bool value);

    void collectAffectedEdges(IdSet& point_ids, IdSet& emtpy_edge_ids);
    void collectAffectedFaces(IdSet& point_ids, IdSet& emtpy_face_ids);

    bool isPointSelect(Id point_id);
    bool checkCullPoint(Id id, vec4 camera_pos);    

// --- Debugging ---
public:
    vec4 getPoint(Id id);
    void printSelect();
    void debug();

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