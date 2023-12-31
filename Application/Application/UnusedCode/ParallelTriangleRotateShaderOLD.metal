#define __IN_SHADER__
#include <metal_stdlib>
#include "shader_typesOLD.h"
using namespace metal;

struct RasterType {
    float4 position [[position]];
    float4 color;
};

vertex RasterType
ParallelTriangleRotate_vertexShader(const uint vidx [[vertex_id]],
                                    const device ParallelTriangleRotate_vertexType* vertices          [[buffer(ParallelTriangleRotate_vertexIndexVertices)]],
                                    constant     float*                             aspect_ratio_ptr [[buffer(ParallelTriangleRotate_vertexIndexAspectRatio)]])
{
    RasterType out;
    
    float  aspect_ratio = *aspect_ratio_ptr;
    
    out.position = vector_float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = vertices[vidx].position.xy * float2(aspect_ratio, 1.0);
    
    out.color = vertices[vidx].color;
    
    return out;
}

fragment float4 ParallelTriangleRotate_fragmentShader(RasterType in [[stage_in]]) {
    return in.color;
}
