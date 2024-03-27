#define __IN_SHADER__
#define __INTERNAL__
#include <metal_stdlib>
#include <simd/simd.h>
#include "shader_types/S_thick_line_types.h"
#include "shader_types/uniform_types.h"

using namespace metal;

typedef UN_FrameData_T          FrameData;
typedef ThickLine_Point_T       C_Point;
typedef INT_ThickLine_Point_T   V_Point;
typedef INT_ThickLine_Cluster_T V_Cluster;


// ================ Trianglize Line ================
void setPoint(device V_Point* point_ar, int idx, float3 pos, float2 uv, float4 color) { 
    V_Point point;
    point.position = pos;
    point.uv       = uv;
    point.color    = color;
    point_ar[idx]  = point;
}
 
kernel void
INT_ThickLine_computeShader(             uint       tidx       [[thread_position_in_grid]],
                                  device V_Cluster* cluster    [[buffer(INT_ThickLine_tri_cluster_I)]],
                            const device C_Point*   vertices   [[buffer(INT_ThickLine_vertices_I)]],
                                constant FrameData* frame_data [[buffer(INT_ThickLine_frame_data_I)]])
{
    float  width = 0.01f;

    C_Point tail = vertices[2*tidx];
    C_Point tip  = vertices[2*tidx+1];

    float4 tail_pos = tail.position;
    float4 tip_pos  = tip.position;

    // --- Project points ---
    float4x4 pers_mat = float4x4(frame_data->pers_mat);
    tail_pos = pers_mat * tail_pos;
    tail_pos = tail_pos/tail_pos.w;
    tip_pos  = pers_mat * tip_pos;
    tip_pos  = tip_pos/tip_pos.w;

    // --- Parallel and perpendicular ---
    float3 par  = width * float3( normalize(tip_pos.xy - tail_pos.xy) , 0.0);
    float3 perp = float3( par.y, -par.x, 0.0); 

    float3 bl, br, tl, tr;
    // --- Tail End ---
    bl = tail_pos.xyz + perp; 
    br = tail_pos.xyz - perp;
    tl = bl - par;
    tr = br - par;

    setPoint(cluster[tidx].tail_end, 0, bl, float2(-1.0, 0.0), tail.color);
    setPoint(cluster[tidx].tail_end, 1, br, float2( 1.0, 0.0), tail.color);
    setPoint(cluster[tidx].tail_end, 2, tr, float2( 1.0, 1.0), tail.color);

    setPoint(cluster[tidx].tail_end, 3, tr, float2( 1.0, 1.0), tail.color);
    setPoint(cluster[tidx].tail_end, 4, tl, float2(-1.0, 1.0), tail.color);
    setPoint(cluster[tidx].tail_end, 5, bl, float2(-1.0, 0.0), tail.color);

    // --- Center ---
    tl = br;
    br = tip_pos.xyz + perp;
    tr = tip_pos.xyz - perp;

    setPoint(cluster[tidx].center, 0, bl, float2( 0.0, 0.0), tail.color);
    setPoint(cluster[tidx].center, 1, br, float2( 0.0, 0.0), tip.color);
    setPoint(cluster[tidx].center, 2, tr, float2( 0.0, 0.0), tip.color);

    setPoint(cluster[tidx].center, 3, tr, float2( 0.0, 0.0), tip.color);
    setPoint(cluster[tidx].center, 4, tl, float2( 0.0, 0.0), tail.color);
    setPoint(cluster[tidx].center, 5, bl, float2( 0.0, 0.0), tail.color);

    // --- Tip End ---
    bl = tr;
    tr = br + par;
    tl = bl + par;

    setPoint(cluster[tidx].tip_end, 0, bl, float2(-1.0, 0.0), tip.color);
    setPoint(cluster[tidx].tip_end, 1, br, float2( 1.0, 0.0), tip.color);
    setPoint(cluster[tidx].tip_end, 2, tr, float2( 1.0, 1.0), tip.color);

    setPoint(cluster[tidx].tip_end, 3, tr, float2( 1.0, 1.0), tip.color);
    setPoint(cluster[tidx].tip_end, 4, tl, float2(-1.0, 1.0), tip.color);
    setPoint(cluster[tidx].tip_end, 5, bl, float2(-1.0, 0.0), tip.color);
}
