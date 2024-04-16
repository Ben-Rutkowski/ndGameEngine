#import "RenderSwitcher.h"
#import "draw_routine_indices.h"
#import "DrawRoutines.h"

@implementation RenderSwitcher
{
//    --- Metal Componenets ---
    id<MTLDevice>       _device;
    id<MTLCommandQueue> _command_queue;
    id<MTLLibrary>      _library;
    MTLPixelFormat      _view_pixel_format;
    
//    --- Routines ---
    NSMutableArray<DrawRoutineTemplate<DrawRoutineProtocol>*>* _loaded_draw_routines;
    DrawRoutineTemplate<DrawRoutineProtocol>* _armed_draw_routine;
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
        
        NSUInteger null_index = [self createDrawRoutine:DrawRoutineKindNull];
        [self armRoutine:null_index];
    }
    return self;
}

// ==== Routine Interface ====
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind {
    DrawRoutineTemplate<DrawRoutineProtocol>* routine = nil;
    
    switch (draw_routine_kind) {
        case DrawRoutineKindNull: {
            NSLog(@"Creating Null Routine");
            routine = [[NullDrawRoutine alloc]
                       initWithDevice:_device
                         commandQueue:_command_queue
                              library:_library
                          pixelFormat:_view_pixel_format];
            break;
        }
            
        case DrawRoutineKindDebug: {
            NSLog(@"Creating Line Routine");
            routine = [[DrawingLines alloc]
                       initWithDevice:_device
                         commandQueue:_command_queue
                              library:_library
                          pixelFormat:_view_pixel_format];
            break;
        }

        case DrawRoutineTriangle: {
            NSLog(@"Creating Triangle Routine");
            routine = [[DrawTriangle alloc]
                       initWithDevice:_device
                         commandQueue:_command_queue 
                              library:_library 
                          pixelFormat:_view_pixel_format];
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

- (id) getDrawRoutineAtIndex:(NSUInteger)index {
    return _loaded_draw_routines[index];
}

- (void) armRoutine:(NSUInteger)index {
    _armed_draw_routine = _loaded_draw_routines[index];
}


// ==== Draw ====
- (void) drawInMetalLayer:(CAMetalLayer*)metal_layer {
    @autoreleasepool {
        id<CAMetalDrawable> current_drawable = [metal_layer nextDrawable];
        if (current_drawable == nil) {
            return;
        }
        
        id<MTLCommandBuffer> command_buffer = [_command_queue commandBuffer];
        
        [_armed_draw_routine predrawOpenInBuffers];
        [_armed_draw_routine drawInDrawable:current_drawable
                            inCommandBuffer:command_buffer];
        [_armed_draw_routine predrawCloseInBuffers];
        
        __block DrawRoutineTemplate<DrawRoutineProtocol>* block_routine = _armed_draw_routine;
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> nonnull) {
            [block_routine drawCompletedInBuffers];
        }];
        
        [command_buffer presentDrawable:current_drawable];
        [command_buffer commit];
    }
}

@end
