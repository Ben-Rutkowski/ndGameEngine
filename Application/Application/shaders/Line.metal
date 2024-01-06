#define __IN_SHADER__
#define __LINE_INTERNAL_TYPE__
#include <metal_stdlib>
#include "shader_types.h"

struct Line_Triangalized_Raster {
    float4 position [[position]];
    float2 uv;
    float4 color;
};


// ================ Compute Shader ================
kernel void
Line_Internal_computeShader(uint tidx [[thread_position_in_grid]],
                            const device Line_PtVtype* line_vertices [[buffer(Line_PtVidx_vertices)]],
                            device LineINT_TraigClstrVtype* cluster [[buffer(Line_PtVidx_triag_cluster)]])
{
    const float width = 0.05;
    
    Line_PtVtype tail;
    Line_PtVtype tip;
    float2       perp;
    float2       par;
    
    Line_TriagVtype edge_bottom;
    Line_TriagVtype edge_top;
    Line_TriagVtype aux;
    
    tail = line_vertices[2*tidx];
    tip  = line_vertices[2*tidx+1];
    
    par  = width * metal::normalize(tip.position - tail.position);
    perp = width * float2(par.y, -par.x);
    
//    --- Tail End ---
    
    
}


// ================ Render Shaders ================
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
