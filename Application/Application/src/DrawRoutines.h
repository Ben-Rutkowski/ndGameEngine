/*
    Routines and subroutines have no knowledge of the device
    Distict buffer methods for each (sub)routine
*/

#ifndef DRAW_ROUTINES_H
#define DRAW_ROUTINES_H

#define __IN_COCOA__
#import "cocoa_interface.hpp"
#import "DrawRoutineTemplates.h"

/*  Subroutine protocol
    - configure with a pixel format :
        - compile all pipelinestates
        - finalize render pass descrptor
        - run finalize config
 
    - encode subroutine :
        - set render pass to current drawable
        - encode all commands in given command buffer
 
    {
        Private varables :
            - pipelines
            - buffers (they live in the routine class)
            - aux
    }
*/

// ================ SUBROUTINES ================
@interface OLDStaticShapeSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
@end

@interface StaticShapeSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
@end


/*  Routine Protocol
    - init with device and library :
        - init super with device
        - init all subroutines with device and library
    
    - configure with pixel formate :
        - configure all subroutines
 
    - draw in drawable :
        - encode all subroutine commands
        - commit in current drawable
 
     {
         Private varables :
             - subroutines
             - buffers
             - aux
     }
*/

// ================ ROUTINES ================
@interface OLDStaticShapeRoutine : DrawRoutineTemplate<DrawRoutineProtocol>
@end

@interface StaticShapeRoutine : DrawRoutineTemplate<DrawRoutineProtocol>
@end

#endif
