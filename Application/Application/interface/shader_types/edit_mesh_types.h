#ifndef EDIT_MESH_TYPES_H
#define EDIT_MESH_TYPES_H

#include "cocoa_vector_types.h"


// ================ Enums ================


// ================ Types ================ 
#pragma pack(push,1)
typedef struct EditMesh_Point_T {
    vec4 position;
} EditMesh_Point_T;

typedef struct EditMesh_Edge_T {
    vec4 position;
} EditMesh_Edge_T;

typedef struct EditMesh_Tri_T {
    vec4 position;
} EditMesh_Tri_T;
#pragma pack(pop)

#endif
