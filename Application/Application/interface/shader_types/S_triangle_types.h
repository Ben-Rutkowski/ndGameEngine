#ifndef S_TRIANGLE_TYPES_H
#define S_TRIANGLE_TYPES_H

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

typedef enum Triangle_I {
    tri_vertices_I = 0,
    tri_pers_mat_I  = 1
} Triangle_I;

// ================ Shader Types ================
#pragma pack(push,1)

typedef struct TrianglePoint_T {
    VECTOR_4F position;
    VECTOR_4F color;
} TrianglePoint_T;

#pragma pack(pop)

#endif
