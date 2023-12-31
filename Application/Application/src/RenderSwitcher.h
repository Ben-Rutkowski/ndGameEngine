#ifndef RENDER_SWITCHER_COCOA_H
#define RENDER_SWITCHER_COCOA_H

#import <MetalKit/MetalKit.h>

@interface RenderSwitcher : NSObject
- (nonnull instancetype) initWithMTLDevice:(nonnull id<MTLDevice>)device
                                metalLayer:(nonnull CAMetalLayer*)layer;

// --- Initializing ---
- (unsigned int) initDrawRoutine:(unsigned int)draw_routine_kind;

// --- Drawing ---
- (void) drawInMetalLayer:(nonnull CAMetalLayer*)metal_layer;
@end

#endif
 
