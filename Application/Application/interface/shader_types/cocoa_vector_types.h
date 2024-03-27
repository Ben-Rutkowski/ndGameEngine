#ifndef COCOA_VECTOR_TYPES_H
#define COCOA_VECTOR_TYPES_H

#ifndef __ND_VECTOR__
#import <simd/simd.h>

typedef vector_int2 vec2i;
typedef vector_int4 vec4i;

typedef vector_float2 vec2;
typedef vector_float4 vec4;

typedef matrix_float2x2 mat2;
typedef matrix_float4x4 mat4;
#endif

#endif
