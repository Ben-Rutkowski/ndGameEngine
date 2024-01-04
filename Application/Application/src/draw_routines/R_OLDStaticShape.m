#import "DrawRoutines.h"

@implementation OLDStaticShapeRoutine
{
//    --- Subroutines ---
    OLDStaticShapeSubroutine* _static_shape_subroutine;
    
//    --- Resources ---
    id<MTLBuffer> _vertex_data_buffer;
    NSUInteger    _vertex_count;
}

// ==== Configure ====
- (instancetype) initWithDevice:(nonnull id<MTLDevice>)device
                        library:(nonnull id<MTLLibrary>)library
{
    self = [super initWithDevice:device];
    if (self) {
        _static_shape_subroutine = [[OLDStaticShapeSubroutine alloc] initWithDevice:device
                                                                            library:library];
    }
    return self;
}

- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {
    NSLog(@"Configuring Routine");
    [_static_shape_subroutine configureWithDrawablePixelFormat:pixel_format];
}


// ==== Resources ====
- (void) bindBuffer:(NSUInteger)buffer_index {}

- (void) createBufferWithVertexCount:(NSUInteger)count {
    NSLog(@"Create Buffer");
    _vertex_data_buffer = [self OLDnewSharedBufferWithLength:count*sizeof(StaticShape_VertexType)];
    [_static_shape_subroutine linkBufferOLD:_vertex_data_buffer vertexCount:count];
}

- (DynamicBuffer *) getBuffer { return nil; }

- (id<MTLBuffer>) getBufferOLD {
    return _vertex_data_buffer;
}

- (void) expandBufferToSize:(NSUInteger)new_size {
    
}


// ==== Drawing ====
- (void) OLDdrawInDrawable:(nonnull id<CAMetalDrawable>)drawable
        inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
{
    [_static_shape_subroutine encodeSubroutineInBuffer:command_buffer 
                                             inTexture:drawable.texture];
    
    [command_buffer presentDrawable:drawable];
    [command_buffer commit];
}

- (void)beforeDraw {NSLog(@"Before Draw");}
- (void)drawUntapScheduled {NSLog(@"Schedule");}
- (void)drawUntapCompleted {NSLog(@"Completed");}

- (void)drawInDrawable:(nonnull id<CAMetalDrawable>)drawable 
       inCommandBuffer:(nonnull id<MTLCommandBuffer>)command_buffer 
{
    [_static_shape_subroutine encodeSubroutineInBuffer:command_buffer
                                             inTexture:drawable.texture];
    
    [command_buffer presentDrawable:drawable];
}


@end
