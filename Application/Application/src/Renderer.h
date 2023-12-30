#ifndef RENDERER_COCOA_H
#define RENDERER_COCOA_H

#import <MetalKit/MetalKit.h>

@interface Renderer : NSObject
- (nonnull instancetype) initWithMTLDevice:(nonnull id<MTLDevice>)device drawablePixelFormat:(MTLPixelFormat) pixel_format;
- (void) drawInMetalLayer:(nonnull CAMetalLayer*)metal_layer;
@end

#endif
 
