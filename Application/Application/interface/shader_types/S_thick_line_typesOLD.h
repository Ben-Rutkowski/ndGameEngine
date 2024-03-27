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

typedef enum ThickLine_I_OLD {
    vertices_I     = 0,
    aspect_ratio_I = 1
} ThickLine_I_OLD;

#ifdef __INTERNAL__
typedef enum INT_Compute_I {
    INT_vertices_I       = 0,
    INT_triang_cluster_I = 1,
    INT_aspect_ratio_I   = 2
} INT_Compute_I;
#endif

// ================ Shader Types ================
#pragma pack(push,1)

typedef struct ThickLinePoint_T {
    VECTOR_2F position;
    VECTOR_4F color;
} ThickLinePoint_T;

#ifdef __INTERNAL__
typedef struct INT_Point_T {
    VECTOR_2F position;
    VECTOR_2F uv;
    VECTOR_4F color;
} INT_Point_T;

typedef struct INT_Cluster_T {
    INT_Point_T tail_end[6];
    INT_Point_T center[6];
    INT_Point_T tip_end[6];
} INT_Cluster_T;
#endif

#pragma pack(pop)

#endif
