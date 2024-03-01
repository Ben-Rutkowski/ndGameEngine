#ifndef ND_VIEW_H
#define ND_VIEW_H

#import <AppKit/AppKit.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

#import "RenderSwitcher.h"

@interface ndView : NSView<CALayerDelegate>
// --- Initialization ---
- (nonnull instancetype) initWithFrame:(CGRect)frame device:(nonnull id<MTLDevice>)device;
- (void) setRenderSwitcher:(nonnull RenderSwitcher*)render_switcher;

// --- Configure ---
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind;
- (CGSize) getWindowSize;

// --- Draw ---
- (void) draw;
@end

#endif
