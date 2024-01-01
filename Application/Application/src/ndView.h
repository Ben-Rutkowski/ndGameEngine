#ifndef ND_VIEW_H
#define ND_VIEW_H

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

#import "RenderSwitcher.h"

@interface ndView : NSView<CALayerDelegate>
- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device;

- (void) setRenderSwitcher:(nonnull RenderSwitcher*)render_switcher;

- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind;

- (void) draw;
@end

#endif
