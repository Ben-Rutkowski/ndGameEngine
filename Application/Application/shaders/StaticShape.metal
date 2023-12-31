#define __IN_SHADER__
#include <metal_stdlib>
#include "shader_types.h"
using namespace metal;

struct RasterData {
    float4 position [[position]];
    float4 color;
};

vertex 
RasterData StaticShape_vertexShader(const uint vidx [[vertex_id]],
                                    const device StaticShape_VertexType* vertices [[buffer(StaticShape_VertexIndex_vertices)]],
                                    constant float* aspect_ratio_ptr [[buffer(StaticShape_VertexIndex_aspect_ratio)]])
{
    RasterData out;
    float aspect_ratio = *aspect_ratio_ptr;
    
    out.position    = float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = vertices[vidx].position * float2(aspect_ratio, 1.0);
    out.color       = vertices[vidx].color;
    
    return out;
}

fragment float4 StaticShape_fragmentShader(RasterData in [[stage_in]]) {
    return in.color;
}
