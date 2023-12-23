#ifndef BASIC_TRIANGLE_TYPES_H
#define BASIC_TRIANGLE_TYPES_H

#ifndef __IN_SHADER__
#import <MetalKit/MetalKit.h>
#endif

typedef enum BasicTriangle_VertexIndex{
    BasicTriangle_VertexIndexVertices = 0
//    BasicTriangle_VertexIndexAspectRatio = 1
} BasicTriangle_VertexIndex;

typedef struct {
    vector_float2 position;
    vector_float4 color;
} BasicTriangle_VertexType;

#endif
