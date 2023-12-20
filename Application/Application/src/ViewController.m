#import <MetalKit/MetalKit.h>
#import <Metal/Metal.h>

#import "ViewController.h"
#import "Renderer.h"

@implementation ViewController
{
    MTKView* _mtk_view;
    Renderer* _renderer;
}

- (void) loadView {
    NSLog(@"Loading View");
    self.view = [[MTKView alloc] initWithFrame:NSMakeRect(0, 0, 800, 600)
                                        device:MTLCreateSystemDefaultDevice()];
    [self.view setWantsLayer:YES];
    [self.view setAutoresizingMask: NSViewWidthSizable | NSViewHeightSizable ];
    NSLog(@"Finished Loading View");
}

- (void) viewDidLoad {
    NSLog(@"View Did Load");
    [super viewDidLoad];
    
    _mtk_view = (MTKView*)self.view;
    _mtk_view.enableSetNeedsDisplay = YES;
    _mtk_view.clearColor = MTLClearColorMake(1.0, 0.0, 0.0, 1.0);
    
    _renderer = [[Renderer alloc] initWithMetalKitView:_mtk_view];
    if (!_renderer) {
        NSLog(@"Renderer initialization failed");
        return;
    }
    
    [_renderer mtkView:_mtk_view drawableSizeWillChange:_mtk_view.drawableSize];
    [_mtk_view setDelegate:_renderer];
    
    NSLog(@"View Finished Up");
}

@end
