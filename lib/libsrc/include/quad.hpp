#include "matrix.hpp"

#ifndef QUAD_HPP
#define QUAD_HPP

/* CLASS EditVertex

A BodyVertex is a vec4 whose w component is always 1.0f.

======== ATTRIBUTES ========
- vec4 vertex


======== METHODS ========
-------- CONSTRUCTORS --------
- EditVertex : Creates a EditVertex with the given vec3
    ==Parameters==
        - vec3 vector

- EditVertex : Creates a EditVertex with the given arguments
    ==Parameters==
        - float arg0
        - float arg1
        - float arg2

-------- GETS AND SETS --------
- getVertex : returns the vertex vector
    ==Return==
        - vec4

-------- TRANSFORMATION --------
- transform : Takes a transformation mat4 and multiples it to vertex.
    ==Parameters==
        - mat4 trans

*/

class EditVertex {
// --- Attributes ---
private:
    vec4 vertex;

// --- Constructor ---
public:
    EditVertex(vec3 v);
    EditVertex(float arg0, float arg1, float arg2);

// --- Gets and Sets ---
public:
    vec4 get();

// --- Transformations ---
public:
    void transform(mat4 trans);
};

/* CLASS Quad

A Quad is a set of indices that reference BodyVertices in a Body.

A Quad has a bottom-left, bottom-right, top-right, top-left index,
(BL, BR, TR, RL). The two tris that are rendered are BL-BR-TR and 
TR-TL-BL. The normal for the face is calculated as the average of
the two triangle normals.

======== ATTRIBUTES ========
- unsigned int BL, BR, TR, TL : the indeices for the four corners


======== METHODS ========
-------- CONSTRUCTOR --------
- Quad : Assigns corner indecies.

-------- GETS AND SETS --------
- get : Returns the vertex id of the given corner
    ==Parameter==
        - qCorner
    ==Return==
        - unsigned int

- getEdge : Returns the edge vec4 of the edge of the Quad (CCW)
    ==Parameters==
        - qEdge
    ==Return==
        - vec4

- set : Sets the given corner with a given vertex id
    ==Parameters==
        - qCorner
        - unsigned int

-------- CALCULATIONS --------
- normalTri : Calculates the normal of the triangle from the two given edges.

- normal : Calculates a normal for the face by 
    averaging the normal for each tri.

*/

enum class qCorner { BL, BR, TR, TL };
enum class qEdge   { B, R, T, L };

typedef std::vector<EditVertex> VCache;

class Quad {
// --- Attributes ---
private:
    std::array<unsigned int, 4> corners;

// --- Constructor ---
public:
    Quad(std::array<unsigned int, 4>& corner_in);
    Quad(std::array<unsigned int, 4>&& corner_in);

// --- Gets and Sets ---
public:
    unsigned int get(qCorner corn);
    void set(qCorner corn, unsigned int id); 
    vec4 getEdge(qEdge edge, VCache& vertices);

// --- Calculations ---
public:
    vec4 normalTri(qEdge edge0, qEdge edge1, VCache& vertices);
    vec4 normal(VCache& vertices);

// --- Private ---
private:
    unsigned int tipID(qEdge edge);
    unsigned int tailID(qEdge edge);

};

#endif