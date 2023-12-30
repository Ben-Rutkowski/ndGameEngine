#import "ndView.h"
#import "Renderer.h"

@implementation ndView
{
    id<MTLDevice> _device;
    CAMetalLayer* _metal_layer;
    
    Renderer* _renderer;
}

- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device {
    self = [super initWithFrame:frame];
    
    if (self) {
        self.wantsLayer = YES;
        self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;
        self.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        
        _metal_layer = (CAMetalLayer*)self.layer;
        self.layer.delegate = self;
        
        _device = device;
        _metal_layer.device = device;
        _metal_layer.pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        _metal_layer.displaySyncEnabled = NO;
        
        _renderer = [[Renderer alloc] initWithMTLDevice:device drawablePixelFormat:_metal_layer.pixelFormat];
    }
    
    return self;
}

- (CALayer*) makeBackingLayer {
    return [CAMetalLayer layer];
}

- (void) viewDidMoveToWindow {
    [super viewDidMoveToWindow];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
}

- (void) resizeDrawableScale:(CGFloat)scale_factor {
    CGSize new_size = self.bounds.size;
    new_size.width  *= scale_factor;
    new_size.height *= scale_factor;
    
    if (new_size.width <= 0 || new_size.height <= 0) {
        return;
    }
    
    if (new_size.width  == _metal_layer.drawableSize.width &&
        new_size.height == _metal_layer.drawableSize.height)
    {
        return;
    }
    
    _metal_layer.drawableSize = new_size;
}

- (void) render {
    [_renderer drawInMetalLayer:_metal_layer];
    
}

// ================ CAMetalLayer Callbacks ================
- (void) displayLayer:(CALayer*)layer {
    [self render];
}

- (void) drawLayer:(CALayer*)layer inContext:(CGContextRef)ctx {
    [self render];
}

- (void) drawRect:(CGRect)rect {
    [self render];
}

// ================ NSView Functions ================
- (void) viewDidChangeBackingProperties {
    [super viewDidChangeBackingProperties];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
}

- (void) setFrameSize:(NSSize)size {
    [super setFrameSize:size];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
}

- (void) setBoundsSize:(NSSize)size {
    [super setBoundsSize:size];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
}

@end
