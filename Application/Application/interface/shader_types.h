#ifndef SHADER_TYPES_H
#define SHADER_TYPES_H

#ifndef __IN_SHADER__
#ifndef __ND_VECTOR__
#import <MetalKit/MetalKit.h>
#endif
#endif

#ifdef __ND_VECTOR__
#define VECTOR_2F vec2
#define VECTOR_3F vec3
#define VECTOR_4F vec4
#else
#define VECTOR_2F simd_float2
#define VECTOR_3F simd_float3
#define VECTOR_4F simd_float4
#endif


// ======== EditMesh ========
typedef struct V_EditMesh_pt {
    VECTOR_4F position;
    VECTOR_4F color;
} V_EditMesh_pt;

typedef struct V_EditMesh_tri {
    VECTOR_4F position;
    VECTOR_4F color;
} V_EditMesh_tri;
// ======== EditMesh ========


// ======== Line ========
#pragma pack(push,1)
// --- Line_Point_VertexType --- : vertices of the lines
typedef struct Line_Point_VertexType {
    VECTOR_2F position; // - Position of end
    VECTOR_4F color;    // - Color of end
} Line_PtVtype;

typedef struct Line_Point_VertexUniform {
    float line_width;
} Line_Vtype_Uniform;

// --- Line_Triangalized_VertexType --- : vertices of the triangalization of a line
typedef struct Line_Triangalized_VertexType {
    VECTOR_2F position; // - Position of the vertex
    VECTOR_2F uv;       // - UV coordinate for curved edged in fragment shader
    VECTOR_4F color;    // - Color of the vertex
} Line_TriagVtype;

#ifdef __LINE_INTERNAL_TYPE__
typedef struct LineINTERNAL_TrianglizedCluster_VertexType {
    Line_TriagVtype center[6];
    Line_TriagVtype tail_end[6];
    Line_TriagVtype tip_end[6];
} LineINT_TraigClstrVtype;
#endif
#pragma pack(pop)

typedef enum Line_Point_VertexIndex {
    Line_PtVidx_vertices      = 0,
    Line_PtVidx_triag_cluster = 1
} Line_Point_VertexIndex;

typedef enum Line_Triangalization_VertexIndex {
    Line_TriagVidx_vertices     = 0,
    Line_TriagVidx_aspect_ratio = 1
} Line_Triangalization_VertexIndex;
// ======== Line ========

#endif
