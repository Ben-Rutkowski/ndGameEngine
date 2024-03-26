#define __IN_SHADER__
#define __INTERNAL__
#include <simd/simd.h>
#include "shader_types/S_triangle_types.h"
#include "shader_types/uniform_types.h"

// ================ Draw Triangle ================
struct RasterType {
    float4 position [[position]];
    float4 color;
};

vertex RasterType
Triangle_vertexShader(             uint               vidx       [[vertex_id]],
                      const device TrianglePoint_T*   vertices   [[buffer(tri_vertices_I)]],
                          constant UN_FrameDataNew_T* frame_data [[buffer(tri_pers_mat_I)]])
{
    RasterType out;

    out.position = float4(0.0, 0.0, 0.0, 1.0);
    out.position.xyz = vertices[vidx].position.xyz;
    out.color        = vertices[vidx].color;

    out.position = frame_data->pers_mat * out.position;

    return out;
}

fragment float4 Triangle_fragmentShader(RasterType in [[stage_in]]) {
    // return float4(1.0, 1.0, 1.0, 1.0);
    return in.color;
}
