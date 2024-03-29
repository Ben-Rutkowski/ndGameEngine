#ifndef S_THICK_LINE_TYPES_H
#define S_THICK_LINE_TYPES_H

#include "cocoa_vector_types.h"


// ================ Enums ================
typedef enum ThickLine_I {
    ThickLine_vertices_I   = 0,
    ThickLine_frame_data_I = 1
} ThickLine_I;

#ifdef __INTERNAL__
typedef enum INT_ThickLine_I {
    INT_ThickLine_vertices_I    = 0,
    INT_ThickLine_tri_cluster_I = 1,
    INT_ThickLine_frame_data_I  = 2
} INT_ThickLine_I;
#endif


// ================ Shader Types ================
#pragma pack(push,1)
typedef struct ThickLine_Point_T {
    vec4 position;
    vec4 color;
    float width;
} ThickLine_Point_T;

#ifdef __INTERNAL__
typedef struct INT_ThickLine_Point_T {
    vec4 position;
    vec4 color;
    vec2 uv;
} INT_ThickLine_Point_T;

typedef struct INT_ThickLine_Cluster_T {
    INT_ThickLine_Point_T tail_end[6];
    INT_ThickLine_Point_T center[6];
    INT_ThickLine_Point_T tip_end[6];
} INT_ThickLine_Cluster_T;

typedef struct INT_ThickLine_compute_FrameData_T {
    mat4 view;
    mat4 pers;
} INT_ThickLine_compute_FrameData_T;
#endif
#pragma pack(pop)

#endif
