/*
    // (IDK  even know about this anymore) ((I think it's wrong))!
    // Routines and subroutines have no knowledge of the device
    // Distict buffer methods for each (sub)routine
*/

#ifndef DRAW_ROUTINES_H
#define DRAW_ROUTINES_H

#define __IN_COCOA__
#import "cocoa_interface.hpp"
#import "DrawRoutineTemplates.h"
#import "DrawSubroutineTemplate.h"
#import "shader_types/uniform_types.h"

// ================ SUBROUTINES ================
@protocol DrawSubroutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;

- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture;

@end


// @interface StaticShapeSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
// @end
// 
// @interface LineSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
// @end

@interface ThickLineSubroutine : DrawSubroutineTemplate<DrawSubroutineProtocol>
@end


// ================ ROUTINES ================
@protocol DrawRoutineProtocol
- (nonnull instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                           commandQueue:(nonnull id<MTLCommandQueue>)command_queue
                                library:(nonnull id<MTLLibrary>)library
                            pixelFormat:(MTLPixelFormat)pixel_format;

// --- Draw ---
- (void) drawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer;

@end

@interface NullDrawRoutine : DrawRoutineTemplate<DrawRoutineProtocol>
@end

// @interface StaticShapeRoutine : DrawRoutineTemplate<DrawRoutineProtocol>
// @end

@interface DrawingLines : DrawRoutineTemplate<DrawRoutineProtocol>
@end

#endif
