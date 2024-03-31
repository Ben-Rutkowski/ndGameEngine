#ifndef S_TRIANGLE_TYPES_H
#define S_TRIANGLE_TYPES_H

#include "cocoa_vector_types.h"


// ================ Enums ================
typedef enum Triangle_I {
    tri_vertices_I = 0,
    tri_pers_mat_I  = 1
} Triangle_I;

// ================ Shader Types ================
#pragma pack(push,1)
typedef struct Triangle_Point_T {
    vec4 position;
    vec4 color;
} Triangle_Point_T;

typedef struct UN_FrameData_T {
    mat4 pers_mat;
} UN_FrameData_T;
#pragma pack(pop)

#endif
