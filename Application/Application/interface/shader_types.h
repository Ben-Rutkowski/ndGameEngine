#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#ifndef __IN_SHADER__
#import <MetalKit/MetalKit.h>
#endif

#ifdef __ND_VECTOR__
#define VECTOR_2F vec2
#define VECTOR_4F vec4
#else
#define VECTOR_2F vector_float2
#define VECTOR_4F vector_float4
#endif

// ======== Static Shape ========
typedef struct {
    VECTOR_2F position;
    VECTOR_4F color;
} StaticShape_VertexType;

typedef enum StaticShape_VertexIndex {
    StaticShape_VertexIndex_vertices     = 0,
    StaticShape_VertexIndex_aspect_ratio = 1
} StaticShape_VertexIndex;

#endif
