#define __IN_SHADER__
#define __INTERNAL__
#include <metal_stdlib>
#include <simd/simd.h>
#include "shader_types/S_thick_line_types.h"

using namespace metal;


// ================ Trianglize Line ================
struct ParPerp {
    float4 par;
    float4 perp;
};

INT_ThickLine_Point_T createPoint(float4 pos, float4 color, float2 uv) { 
    INT_ThickLine_Point_T point;

    point.position = pos;
    point.uv       = uv;
    point.color    = color;

    return point;
}

ParPerp calcParPerp(float2 tip, float2 tail, float line_width, float aspect_ratio) {
    ParPerp out;
    float2  v;

    v    = tip - tail;
    v.x *= aspect_ratio;
    v    = line_width * normalize(v);

    out.par  = float4( v.x/aspect_ratio,  v.y, 0.0, 0.0 );
    out.perp = float4( v.y/aspect_ratio, -v.x, 0.0, 0.0 );

    return out;
}


kernel void INT_ThickLine_computeShader 
(
                 uint                               tidx       [[thread_position_in_grid]],
          device INT_ThickLine_Cluster_T*           cluster    [[buffer(INT_ThickLine_tri_cluster_I)]],
    const device ThickLine_Point_T*                 vertices   [[buffer(INT_ThickLine_vertices_I)]],
        constant INT_ThickLine_compute_FrameData_T* frame_data [[buffer(INT_ThickLine_frame_data_I)]]
)

{
    ThickLine_Point_T tail = vertices[2*tidx];
    ThickLine_Point_T tip  = vertices[2*tidx+1];

    float4x4 pers_mat     = float4x4(frame_data->pers_mat);
    float    aspect_ratio = frame_data->aspect_ratio;
    float    width        = tail.width;

    float4 tail_pos = tail.position;
    float4 tip_pos  = tip.position;

    // --- Project points ---
    tail_pos = pers_mat * tail_pos;
    tail_pos = tail_pos/tail_pos.w;
    tip_pos  = pers_mat * tip_pos;
    tip_pos  = tip_pos/tip_pos.w;

    // --- Parallel and perpendicular ---
    ParPerp par_perp = calcParPerp(tip_pos.xy, tail_pos.xy, width, aspect_ratio);
    float4 par  = par_perp.par;
    float4 perp = par_perp.perp;

    float4 bl, br, tl, tr;
    // --- Tail End ---
    bl = tail_pos + perp; 
    br = tail_pos - perp;
    tl = bl - par;
    tr = br - par;

    cluster[tidx].tail_end[0] = createPoint( bl, tail.color, float2(-1.0, 0.0) );
    cluster[tidx].tail_end[1] = createPoint( br, tail.color, float2( 1.0, 0.0) );
    cluster[tidx].tail_end[2] = createPoint( tr, tail.color, float2( 1.0, 1.0) );

    cluster[tidx].tail_end[3] = createPoint( tr, tail.color, float2( 1.0, 1.0) );
    cluster[tidx].tail_end[4] = createPoint( tl, tail.color, float2(-1.0, 1.0) );
    cluster[tidx].tail_end[5] = createPoint( bl, tail.color, float2(-1.0, 0.0) );

    // --- Center ---
    tl = br;
    br = tip_pos + perp;
    tr = tip_pos - perp;

    cluster[tidx].center[0] = createPoint( bl, tail.color, float2( 0.0, 0.0) );
    cluster[tidx].center[1] = createPoint( br, tip.color,  float2( 0.0, 0.0) );
    cluster[tidx].center[2] = createPoint( tr, tip.color,  float2( 0.0, 0.0) );

    cluster[tidx].center[3] = createPoint( tr, tip.color,  float2( 0.0, 0.0) );
    cluster[tidx].center[4] = createPoint( tl, tail.color, float2( 0.0, 0.0) );
    cluster[tidx].center[5] = createPoint( bl, tail.color, float2( 0.0, 0.0) );

    // --- Tip End ---
    bl = tr;
    tr = br + par;
    tl = bl + par;

    cluster[tidx].tip_end[0] = createPoint( bl, tip.color, float2(-1.0, 0.0) );
    cluster[tidx].tip_end[1] = createPoint( br, tip.color, float2( 1.0, 0.0) );
    cluster[tidx].tip_end[2] = createPoint( tr, tip.color, float2( 1.0, 1.0) );

    cluster[tidx].tip_end[3] = createPoint( tr, tip.color, float2( 1.0, 1.0) );
    cluster[tidx].tip_end[4] = createPoint( tl, tip.color, float2(-1.0, 1.0) );
    cluster[tidx].tip_end[5] = createPoint( bl, tip.color, float2(-1.0, 0.0) );
}


// ================ Draw Trianglized Lines ================
struct RasterType {
    float4 position [[position]];
    float4 color;
    float2 uv;
};

vertex RasterType ThickLine_vertexShader
(             
                 uint                               vidx       [[vertex_id]],
    const device INT_ThickLine_Point_T*             vertices   [[buffer(ThickLine_vertices_I)]],
        constant INT_ThickLine_compute_FrameData_T* frame_data [[buffer(ThickLine_frame_data_I)]]
)

{
    RasterType out;

    out.position = vertices[vidx].position;
    out.color    = vertices[vidx].color;
    out.uv       = vertices[vidx].uv;

    return out;
}

fragment float4 ThickLine_fragmentShader(RasterType in [[stage_in]]) {
    in.uv *= in.uv;
    float dist = in.uv[0] + in.uv[1];

    if (dist > 1.0) {
        discard_fragment();
    }

    return in.color;
}
