#ifndef ND_VIEW_H
#define ND_VIEW_H

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

@interface ndView : NSView<CALayerDelegate>
- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device;
//- (void) resizeDrawable:(CGFloat)scale_factor;
//- (void) renderToMetalLayer:(nonnull CAMetalLayer*)metal_layer;
- (void) render;
@end

#endif
