#import "Renderer.h"

@implementation Renderer
{
    id<MTLDevice> _device;
    id<MTLCommandQueue> _command_queue;
}

- (nonnull instancetype) initWithMetalKitView:(MTKView*)mtk_view {
    self = [super init];
    if (self) {
        _device = mtk_view.device;
        _command_queue = [_device newCommandQueue];
    }
    
    return self;
}

- (void)mtkView:(MTKView*)mtk_view drawableSizeWillChange:(CGSize)size {
    NSLog(@"Resizing MTKVeiw");
}

- (void)drawInMTKView:(MTKView*)mtk_view {
    @autoreleasepool {
        MTLRenderPassDescriptor* render_pass_descriptor = mtk_view.currentRenderPassDescriptor;
        if (render_pass_descriptor == nil) {
            NSLog(@"Failed To Create Render Pass Descriptor");
            return;
        }
        
        id<MTLCommandBuffer> command_buffer   = [_command_queue commandBuffer];
        id<MTLCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:render_pass_descriptor];
        [command_encoder endEncoding];
        
        id<MTLDrawable> drawable = mtk_view.currentDrawable;
        
        [command_buffer presentDrawable:drawable];
        [command_buffer commit];
    }
}

@end
