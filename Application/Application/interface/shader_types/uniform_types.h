#ifndef UNIFORM_TYPES_H
#define UNIFORM_TYPES_H

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


// ================ Enums ================
typedef enum Uniforms_I {
    UN_FrameData_I = 0
} Uniforms_I;


// ================ Types ================
typedef struct UN_FrameData_T {
    float aspect_ratio;
} UN_FrameData_T;

#endif
