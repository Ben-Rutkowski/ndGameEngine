#import "RenderSwitcher.h"
#import "DrawRoutines.h"

@implementation RenderSwitcher
{
//    --- Metal Componenets ---
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    id<MTLLibrary>      _library;
    MTLPixelFormat      _view_pixel_format;
    
//    --- Routines ---
    NSMutableArray<id<DrawRoutineProtocol>>* _loaded_draw_routines;
    id<DrawRoutineProtocol> _bound_draw_routine;
    id<DrawRoutineProtocol> _armed_draw_routine;
    
//    --- Debugging ---
    OLDStaticShapeRoutine* _static_shape_routine;
}

// ==== Initialization ====
- (nonnull instancetype) initWithMTLDevice:(id<MTLDevice>)device
                                metalLayer:(nonnull CAMetalLayer*)layer
{
    self = [super init];
    if (self) {
        _device            = [device retain];
        _command_queue     = [_device newCommandQueue];
        _view_pixel_format = layer.pixelFormat;
        
        _loaded_draw_routines = [[NSMutableArray alloc] init];
        
        NSError* error;
        NSURL* baseurl = [NSURL fileURLWithPath:@"/Users/benjaminrutkowski/"];
        NSURL* url     = [NSURL URLWithString:@"Projects/ndGameEngine/Application/compiled/shader_lib/MetalLibrary.metallib"
                                relativeToURL:baseurl];
        _library = [_device newLibraryWithURL:url error:&error];
        NSAssert(_library, @"Failed to find Metal Library", error);
        
        NSUInteger null_index = [self createDrawRoutine:ndDrawRoutineKindNull];
        [self bindRoutine:null_index];
        [self armRoutine];
    }
    
    return self;
}

// ==== Routine Interface ====
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind {
    id<DrawRoutineProtocol> routine = nil;
    
    switch (draw_routine_kind) {
        case ndDrawRoutineKindNull: {
            NSLog(@"Creating Null Routine");
            routine = [[NullDrawRoutine alloc]
                       initWithDevice:_device
                       library:_library];
            break;
        }
            
        case ndDrawRoutineKindDebug: {
            NSLog(@"Creating Debug Routine");
            routine = [[StaticShapeRoutine alloc]
                       initWithDevice:_device
                       library:_library];
            break;
        }
            
        default: break;
    }
    
    if (routine != nil) {
        [_loaded_draw_routines addObject:routine];
        return [_loaded_draw_routines count] - 1;
    } else {
        NSLog(@"Could Not Create Draw Routine : %lu", draw_routine_kind);
        return 0;
    }
}

- (void) bindRoutine:(NSUInteger)index {
    NSLog(@"Binding routine : %lu", index);
    _bound_draw_routine = _loaded_draw_routines[index];
}

- (void) configureRoutine {
    [_bound_draw_routine configureWithDrawablePixelFormat:_view_pixel_format];
}

- (void) armRoutine {
    NSLog(@"Arming bound routine");
    _armed_draw_routine = _bound_draw_routine;
}


// ==== Inside Routine ====
- (void) bindBuffer:(NSUInteger)index {
    [_bound_draw_routine bindBuffer:index];
}

- (void) createBufferWithVertexCount:(NSUInteger)count {
    [_bound_draw_routine createBufferWithVertexCount:count];
}

- (DynamicBuffer*) getBuffer {
    return [_bound_draw_routine getBuffer];
}


// ==== Draw ====
- (void) drawInMetalLayer:(CAMetalLayer*)metal_layer {
    NSLog(@"DRAW IN RENDER");
    
    @autoreleasepool {
        id<CAMetalDrawable> current_drawable = [metal_layer nextDrawable];
        if (current_drawable == nil) {
            return;
        }
        
        id<MTLCommandBuffer> command_buffer = [_command_queue commandBuffer];
        
        [_armed_draw_routine beforeDraw];
        NSLog(@"-- Prepare Draw on CPU ---");

        [_armed_draw_routine drawInDrawable:current_drawable
                            inCommandBuffer:command_buffer];
        
        __block id<DrawRoutineProtocol> block_routine = _armed_draw_routine;
        [command_buffer addScheduledHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_routine drawUntapScheduled];
            NSLog(@"-- Draw Scheduled on GPU --");
        }];
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            NSLog(@"-- Draw Completed on GPU --");
            [block_routine drawUntapCompleted];
        }];
        
        [command_buffer presentDrawable:current_drawable];
        [command_buffer commit];
    }
}

@end
