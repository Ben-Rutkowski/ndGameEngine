#ifndef RENDER_SWITCHER_COCOA_H
#define RENDER_SWITCHER_COCOA_H

#import <MetalKit/MetalKit.h>

@interface RenderSwitcher : NSObject
// --- Initializing ---
- (nonnull instancetype) initWithMTLDevice:(nonnull id<MTLDevice>)device
                                metalLayer:(nonnull CAMetalLayer*)layer;

// --- Routine Interface ---
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind;
- (void) bindRoutine:(NSUInteger)index;
- (void) configureRoutine;
- (void) armRoutine;

- (void) bindBuffer:(NSUInteger)index;
- (void) createBufferWithVertexCount:(NSUInteger)count;
- (nullable id<MTLBuffer>) getBuffer;

// --- Draw ---
- (void) drawInMetalLayer:(nonnull CAMetalLayer*)metal_layer;

// --- Debugging ---
- (void) debug;
@end

#endif
 
