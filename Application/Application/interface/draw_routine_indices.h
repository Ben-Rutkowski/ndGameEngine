#ifndef DRAW_ROUTINE_INDICES_H
#define DRAW_ROUTINE_INDICES_H

typedef enum Buffer_T {
    DynamicBuffer_T,
    RapidBuffer_T
} Buffer_T;

typedef enum DrawRoutineKind {
    DrawRoutineKindNull  = 0,
    DrawRoutineKindDebug = 1,
    DrawRoutineTriangle  = 2
} DrawRoutineKind;


// ================ Buffer Indices ================
typedef enum DebugDynamicBuffer {
    R_Debug_DynamicBuffer0 = 0,
    R_Debug_DynamicBuffer1 = 1,
    R_Debug_DynamicBuffer2 = 2,
    
    R_Debug_DynamicBufferCount
} DebugDynamicBuffer;

typedef enum TriangleBuffers {
    R_Triangle_Vertices  = 0,
    R_Triangle_FrameData = 1
} TriangleBuffers;

#endif
