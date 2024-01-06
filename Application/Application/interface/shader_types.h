#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#ifndef __IN_SHADER__
#ifndef __ND_VECTOR__
#import <MetalKit/MetalKit.h>
#endif
#endif

#ifdef __ND_VECTOR__
#define VECTOR_2F vec2
#define VECTOR_4F vec4
#else
#define VECTOR_2F simd_float2
#define VECTOR_4F simd_float4
#endif


// ======== Static Shape ========
#pragma pack(push,1)
typedef struct StaticShape_VertexType {
    VECTOR_2F position;
    VECTOR_4F color;
} StaticShapeVtype;
#pragma pack(pop)

typedef enum StaticShape_VertexIndex {
    StaticShapeVidx_vertices     = 0,
    StaticShapeVidx_aspect_ratio = 1
} StaticShape_VertexIndex;
// ======== Static Shape ========


// ======== Line ========
#pragma pack(push,1)
// --- Line_Triangalized_VertexType --- : vertices of the triangalization of a line
typedef struct Line_Triangalized_VertexType {
    VECTOR_2F position; // - Position of the vertex
    VECTOR_2F uv;       // - UV coordinate for curved edged in fragment shader
    VECTOR_4F color;    // - Color of the vertex
} Line_TriagVtype;
#pragma pack(pop)

typedef enum Line_Triangalization_VertexIndex {
    Line_TriagVidx_vertices     = 0,
    Line_TriagVidx_aspect_ratio = 1
} Line_Triangalization_VertexIndex;
// ======== Line ========

#endif
