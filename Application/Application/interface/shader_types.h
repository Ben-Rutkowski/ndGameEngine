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

// ================ Basic Triangle ================
typedef enum BasicTriangle_VertexIndex{
    BasicTriangle_VertexIndexVertices = 0
//    BasicTriangle_VertexIndexAspectRatio = 1
} BasicTriangle_VertexIndex;

typedef struct {
    VECTOR_2F position;
    VECTOR_4F color;
} BasicTriangle_VertexType;

// ================ Parallel Triangle Rotate ================
typedef enum ParallelTriangleRotate_vertexIndex {
    ParallelTriangleRotate_vertexIndexVertices    = 0,
    ParallelTriangleRotate_vertexIndexAspectRatio = 1
} ParallelTriangleRotate_vertexIndex;

typedef struct {
    VECTOR_2F position;
    VECTOR_4F color;
} ParallelTriangleRotate_vertexType;

#endif
