#define __IN_SHADER__
#include <metal_stdlib>
#include "shader_types.h"
using namespace metal;

struct RasterData {
    float4 position [[position]];
    float2 uv;
    float4 color;
};

vertex
RasterData NiceLine_vertexShader(const uint vidx [[vertex_id]],
                                 const device NiceLine_VertexType* vertices [[buffer(NiceLine_VertexIndex_vertices)]],
                                 constant float &aspect_ratio [[buffer(NiceLine_VertexIndex_aspect_ratio)]])
{
    RasterData out;
    
    out.position = float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = vertices[vidx].position * float2(aspect_ratio, 1.0);
    out.uv    = vertices[vidx].uv;
    out.color = vertices[vidx].color;
    
    return out;
}

fragment float4 NiceLine_fragmentShader(RasterData in [[stage_in]]) {
    float distance = in.uv[0]*in.uv[0] + in.uv[1]*in.uv[1];
    if (distance > 1.0) {
        discard_fragment();
    }
    
    return in.color;
}
