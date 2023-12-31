#ifndef ND_VIEW_H
#define ND_VIEW_H

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

@interface ndView : NSView<CALayerDelegate>
- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device;
- (unsigned int) initDrawRoutine:(unsigned int)draw_routine_kind;
- (void) render;
@end

#endif
