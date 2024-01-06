#define __IN_SHADER__
#include <metal_stdlib>
#include "shader_types.h"

struct Line_Triangalized_Raster {
    float4 position [[position]];
    float2 uv;
    float4 color;
};


vertex Line_Triangalized_Raster
Line_Triangalized_vertexShader(uint vidx [[vertex_id]],
                               const device Line_TriagVtype* vertices [[buffer(Line_TriagVidx_vertices)]],
                               constant float* aspect_ratio_ptr [[buffer(Line_TriagVidx_aspect_ratio)]])
{
    Line_Triangalized_Raster out;
    float aspect_ratio = *aspect_ratio_ptr;
    
    out.position    = float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = vertices[vidx].position * float2(aspect_ratio, 1.0);
    
    out.uv    = vertices[vidx].uv;
    out.color = vertices[vidx].color;
    
    return out;
}

fragment float4 Line_Triangalized_fragmentShader(Line_Triangalized_Raster in [[stage_in]]) {
    in.uv *= in.uv;
    float distance = in.uv[0] + in.uv[1];
    
    if (distance > 1.0) {
        metal::discard_fragment();
    }
    
    return in.color;
}
