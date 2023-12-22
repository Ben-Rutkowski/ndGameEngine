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

- (void)mtkView:(nonnull MTKView*)view drawableSizeWillChange:(CGSize)size {
}

- (void)drawInMTKView:(nonnull MTKView*)view {
}

@end
