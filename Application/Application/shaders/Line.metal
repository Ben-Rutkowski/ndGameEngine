#define __IN_SHADER__
#define __LINE_INTERNAL_TYPE__
#include <metal_stdlib>
#include "shader_types/shader_types.h"

struct Line_Triangalized_Raster {
    float4 position [[position]];
    float2 uv;
    float4 color;
};


// ================ Compute Shader ================
kernel void
Line_Internal_computeShader(uint tidx [[thread_position_in_grid]],
                            const device Line_PtVtype* line_vertices [[buffer(Line_PtVidx_vertices)]],
                            device   LineINT_TraigClstrVtype* cluster [[buffer(Line_PtVidx_triag_cluster)]],
                            constant Line_Vtype_Uniform &line_uniform [[buffer(3)]])
{
    const float width = 0.005;
    
    Line_PtVtype tail;
    Line_PtVtype tip;
    float2       perp;
    float2       par;
    
    Line_TriagVtype edge_bottom;
    Line_TriagVtype edge_top;
    
    tail = line_vertices[2*tidx];
    tip  = line_vertices[2*tidx+1];
    
//    par  = line_uniform.line_width * metal::normalize(tip.position - tail.position);
    par  = width * metal::normalize(tip.position - tail.position);
    perp = float2(par.y, -par.x);
    
//    --- Tail End ---
    edge_bottom = { tail.position + perp, float2(-1.0, 0.0), tail.color };
    edge_top    = { tail.position - perp, float2( 1.0, 0.0), tail.color };
    
    cluster[tidx].tail_end[0] = edge_bottom;
    cluster[tidx].tail_end[1] = { edge_bottom.position - par, float2(-1.0, 1.0), tail.color };
    cluster[tidx].tail_end[2] = edge_top;
    
    cluster[tidx].tail_end[3] = { edge_top.position - par, float2(1.0, 1.0), tail.color };
    cluster[tidx].tail_end[4] = edge_top;
    cluster[tidx].tail_end[5] = { edge_bottom.position - par, float2(-1.0, 1.0), tail.color };
    
//    --- Center ---
    edge_bottom.uv = float2(0.0, 0.0);
    edge_top.uv    = float2(0.0, 0.0);
    
    cluster[tidx].center[0] = edge_bottom;
    cluster[tidx].center[1] = edge_top;
    cluster[tidx].center[5] = edge_top;
    
    edge_bottom = { tip.position + perp, float2(0.0, 0.0), tip.color };
    edge_top    = { tip.position - perp, float2(0.0, 0.0), tip.color };
    
    cluster[tidx].center[2] = edge_bottom;
    cluster[tidx].center[3] = edge_top;
    cluster[tidx].center[4] = edge_bottom;
    
//    --- Tip End ---
    edge_bottom.uv = float2( 1.0, 0.0);
    edge_top.uv    = float2(-1.0, 0.0);
    
    cluster[tidx].tip_end[0] = edge_top;
    cluster[tidx].tip_end[1] = { edge_top.position + par, float2(-1.0, 1.0), tip.color };
    cluster[tidx].tip_end[2] = edge_bottom;
    
    cluster[tidx].tip_end[3] = { edge_bottom.position + par, float2(1.0, 1.0), tip.color };
    cluster[tidx].tip_end[4] = edge_bottom;
    cluster[tidx].tip_end[5] = { edge_top.position + par, float2(-1.0, 1.0), tip.color };
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
