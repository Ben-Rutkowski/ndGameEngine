#define __IN_SHADER__
#define __INTERNAL__
#include <metal_stdlib>
#include "shader_types/S_thick_line_types.h"
#include "shader_types/uniform_types.h"

// ================ Trianglize Line ================
kernel void
INT_ThickLine_computeShader(              uint              tidx       [[thread_position_in_grid]],
                                   device INT_Cluster_T*    cluster    [[buffer(INT_triang_cluster_I)]],
                             const device ThickLinePoint_T* vertices   [[buffer(INT_vertices_I)]],
                                 constant UN_FrameData_T*   frame_data [[buffer(INT_aspect_ratio_I)]])
{
    float width        = frame_data[0].thickness;
    float aspect_ratio = frame_data[0].aspect_ratio;

    ThickLinePoint_T tail;
    ThickLinePoint_T tip;

    float2 perp;
    float2 par;

    // --- positions in quad ---
    float2 bl; // bottom left 
    float2 br; // bottom right
    float2 tl; // top left
    float2 tr; // top right 
 
    tail = vertices[2*tidx];
    tip  = vertices[2*tidx+1];

    tail.position *= float2(aspect_ratio, 1.0);
    tip.position  *= float2(aspect_ratio, 1.0);

    par  = width * metal::normalize(tip.position - tail.position);
    perp = float2(par.y, -par.x);

//    --- Tail End ---
    bl = tail.position + perp; 
    br = tail.position - perp; 
    tl = bl - par;
    tr = br - par;

    cluster[tidx].tail_end[0] = { bl, float2(-1.0, 0.0), tail.color };
    cluster[tidx].tail_end[1] = { br, float2( 1.0, 0.0), tail.color };
    cluster[tidx].tail_end[2] = { tr, float2( 1.0, 1.0), tail.color };

    cluster[tidx].tail_end[3] = { tr, float2( 1.0, 1.0), tail.color };
    cluster[tidx].tail_end[4] = { tl, float2(-1.0, 1.0), tail.color };
    cluster[tidx].tail_end[5] = { bl, float2(-1.0, 0.0), tail.color };

//    --- Center ---
    tl = br;
    br = tip.position + perp;
    tr = tip.position - perp;

    cluster[tidx].center[0] = { bl, float2(0.0, 0.0), tail.color };
    cluster[tidx].center[1] = { br, float2(0.0, 0.0), tip.color };
    cluster[tidx].center[2] = { tr, float2(0.0, 0.0), tip.color };

    cluster[tidx].center[3] = { tr, float2(0.0, 0.0), tip.color };
    cluster[tidx].center[4] = { tl, float2(0.0, 0.0), tail.color };
    cluster[tidx].center[5] = { bl, float2(0.0, 0.0), tail.color };

//    --- Tip End ---
    bl = tr;
    tr = br + par;
    tl = bl + par;

    cluster[tidx].tip_end[0] = { bl, float2(-1.0, 0.0), tip.color };
    cluster[tidx].tip_end[1] = { br, float2( 1.0, 0.0), tip.color };
    cluster[tidx].tip_end[2] = { tr, float2( 1.0, 1.0), tip.color };

    cluster[tidx].tip_end[3] = { tr, float2( 1.0, 1.0), tip.color };
    cluster[tidx].tip_end[4] = { tl, float2(-1.0, 1.0), tip.color };
    cluster[tidx].tip_end[5] = { bl, float2(-1.0, 0.0), tip.color };
}


// ================ Draw Trianglized Lines ================
struct RasterType {
    float4 position [[position]];
    float4 color;
    float2 uv;
};


vertex RasterType
ThickLine_vertexShader(             uint            vidx       [[vertex_id]],
                       const device INT_Point_T*    vertices   [[buffer(vertices_I)]],
                           constant UN_FrameData_T* frame_data [[buffer(aspect_ratio_I)]])
{
    RasterType out;

    out.position = float4(vertices[vidx].position, 0.0, 1.0);
    out.color    = vertices[vidx].color;
    out.uv       = vertices[vidx].uv;

    out.position.x *= 1.0/frame_data[0].aspect_ratio;

    return out;
}


fragment float4 ThickLine_fragmentShader(RasterType in [[stage_in]]) {
    in.uv *= in.uv;
    float distance = in.uv[0] + in.uv[1];

    if (distance > 1.0) {
        metal::discard_fragment();
    }

    return in.color;
}
