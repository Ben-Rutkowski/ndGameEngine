#include "quad.hpp"
#include "vertex_buffer_interface.hpp"

#ifndef EDIT_MESH_HPP
#define EDIT_MESH_HPP

/* CLASS EditMesh
    Parent : VertexBufferInterface

An EditMesh is a set of vertices and Quads (SOON N-FACES) that
is able to be manipulated and merged with other EditMeshes to
model 3D game objects.

An edit mesh has a vector of EditVertices and Quads. The EditVertices
are formated as vertex data to draw and the Quads are formatted as
element indices to draw. 

DRAWING :
Vertex Data : | vec4 position |
Index Data  : | uInt quad_idices |

======== Attributes ========
...
- VCache v_cache
- QCache q_cache


======== METHODS ========
-------- Constructors --------
- EditMesh : Creates an EditMesh with the given vertices and quads
    ==Parameters==
        - VCache& vertices
        - VCache& quads

-------- Rendering --------
- loadData : Loads the vertex data into OpenGL

- Draw : Draws mesh 
    ==Parameter==
        - ShaderProgram program

-------- Static --------
- static EditMesh square : Creates a square Quad in the xy-plane
*/

class EditMesh : public VertexBufferInterface {
// --- Attributes ---
public:
    VCache v_cache;
    QCache q_cache;

// --- Constructors ---
public:
    EditMesh(VCache& vertices, QCache& quads);

// --- Rendering ---
public:
    void loadData();
    void draw(ShaderProgram& program);

// --- Private ---
private:
    int vCacheSize();
    int qCacheSize();

// --- Static ---
public:
    static EditMesh square();
};

#endif