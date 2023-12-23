#define __IN_SHADER__
#include <metal_stdlib>
#include "BasicTraigleTypes.h"
using namespace metal;

// ================ RasterType ================
struct RasterType {
    float4 position [[position]];
    float4 color;
};

// ================ Shaders ================
vertex
RasterType BasicTriangle_vertexShader(const uint vidx [[vertex_id]],
                                      const device BasicTriangle_VertexType* vertcies [[buffer(BasicTriangle_VertexIndexVertices)]])
{
    RasterType out;
    
    out.position = float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = vertcies[vidx].position;
    
    out.color = vertcies[vidx].color;
    
    return out;
}

fragment float4 BasicTriangle_fragmentShader(RasterType in [[stage_in]]) {
    return in.color;
}
