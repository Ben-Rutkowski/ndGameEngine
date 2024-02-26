#ifndef DRAW_ROUTINE_INDICES_H
#define DRAW_ROUTINE_INDICES_H

typedef enum DrawRoutineKind {
    DrawRoutineKindNull  = 0,
    DrawRoutineKindDebug = 1
} DrawRoutineKind;

typedef enum DrawingLineBuffer {
    R_Debug_Buffer0 = 0,
    R_Debug_Buffer1 = 1,
    R_Debug_Buffer2 = 2,
    
    R_Debug_BufferCount
} DrawingLineBuffer;

#endif
