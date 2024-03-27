#ifndef S_THICK_LINE_TYPES_H
#define S_THICK_LINE_TYPES_H

#ifndef __IN_SHADER__
#ifndef __ND_VECTOR__
#import <MetalKit/MetalKit.h>
#endif
#endif

#ifdef __ND_VECTOR__
#define VECTOR_2F vec2
#define VECTOR_3F vec3
#define VECTOR_4F vec4
#else
#define VECTOR_2F simd_float2
#define VECTOR_3F simd_float3
#define VECTOR_4F simd_float4
#endif


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
    VECTOR_4F position;
    VECTOR_4F color;
} ThickLine_Point_T;

#ifdef __INTERNAL__
typedef struct INT_ThickLine_Point_T {
    VECTOR_4F color;
    VECTOR_3F position;
    VECTOR_2F uv;
} INT_ThickLine_Point_T;

typedef struct INT_ThickLine_Cluster_T {
    INT_ThickLine_Point_T tail_end[6];
    INT_ThickLine_Point_T center[6];
    INT_ThickLine_Point_T tip_end[6];
} INT_ThickLine_Cluster_T;
#endif

#pragma pack(pop)

#endif
