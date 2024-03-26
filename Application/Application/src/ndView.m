#define __IN_COCOA__
#import "cocoa_interface.hpp"
#import "ndView.h"
#import "RenderSwitcher.h"

@implementation ndView
{
    CAMetalLayer*   _metal_layer;
    RenderSwitcher* _render_switcher;
}


// ==== Initialization ====
- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device {
    self = [super initWithFrame:frame];
    if (self) {
        self.wantsLayer                = YES;
        self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;
        self.autoresizingMask          = NSViewWidthSizable | NSViewHeightSizable;
        
        _metal_layer        = (CAMetalLayer*)self.layer;
        self.layer.delegate = self;
        
        _metal_layer.device             = device;
        _metal_layer.pixelFormat        = MTLPixelFormatBGRA8Unorm_sRGB;
        _metal_layer.displaySyncEnabled = NO;
    }
    
    return self;
}

- (void) setRenderSwitcher:(RenderSwitcher*)render_switcher {
    _render_switcher = render_switcher;
}


// ==== Configure ====
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind {
    return [_render_switcher createDrawRoutine:draw_routine_kind];
}

- (CGSize) getWindowSize {
    return _metal_layer.drawableSize;
}


// ==== Size ====
- (void) resizeDrawableScale:(CGFloat)scale_factor {
    CGSize new_size  = self.bounds.size;
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


// ==== Draw ====
- (void) draw {
    [_render_switcher drawInMetalLayer:_metal_layer];
}


// ================ CAMetalLayer Callbacks ================
- (void) displayLayer:(CALayer*)layer {
    [self draw];
}

- (void) drawLayer:(CALayer*)layer inContext:(CGContextRef)ctx {
    [self draw];
}

- (void) drawRect:(CGRect)rect {
    [self draw];
}

// ================ NSView Functions ================
- (CALayer*) makeBackingLayer {
    return [CAMetalLayer layer];
}

- (void) viewDidMoveToWindow {
    [super viewDidMoveToWindow];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
    // CGSize size_new = [self getWindowSize];
    // int width  = (int)size_new.width;
    // int height = (int)size_new.height;
    // [ndCallBackCenter callback:CocoaCB_Resize 
    //                 withIntOne:width 
    //                 withIntTwo:height];
}

- (void) viewDidChangeBackingProperties {
    [super viewDidChangeBackingProperties];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
    CGSize size_new = [self getWindowSize];
    int width  = (int)size_new.width;
    int height = (int)size_new.height;
    [ndCallBackCenter callback:CocoaCB_Resize 
                    withIntOne:width 
                    withIntTwo:height];
}


- (void) setFrameSize:(NSSize)size {
    [super setFrameSize:size];
    [self resizeDrawableScale:self.window.screen.backingScaleFactor];
    CGSize size_new = [self getWindowSize];
    int width  = (int)size_new.width;
    int height = (int)size_new.height;
    [ndCallBackCenter callback:CocoaCB_Resize 
                    withIntOne:width 
                    withIntTwo:height];
}

- (void) setBoundsSize:(NSSize)size {
    [super setBoundsSize:size];
    // [self resizeDrawableScale:self.window.screen.backingScaleFactor];
    // CGSize size_new = [self getWindowSize];
    // int width  = (int)size_new.width;
    // int height = (int)size_new.height;
    // [ndCallBackCenter callback:CocoaCB_Resize 
    //                 withIntOne:width 
    //                 withIntTwo:height];
}

@end
