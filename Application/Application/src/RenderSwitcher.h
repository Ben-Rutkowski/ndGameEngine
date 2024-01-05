#ifndef RENDER_SWITCHER_COCOA_H
#define RENDER_SWITCHER_COCOA_H

#import "Buffers.h"
#import <MetalKit/MetalKit.h>

@interface RenderSwitcher : NSObject
// --- Initializing ---
- (nonnull instancetype) initWithMTLDevice:(nonnull id<MTLDevice>)device
                                metalLayer:(nonnull CAMetalLayer*)layer;

// --- Routine Interface ---
- (NSUInteger) createDrawRoutine:(NSUInteger)draw_routine_kind;
- (nonnull id) getDrawRoutineAtIndex:(NSUInteger)index;
- (void) bindRoutine:(NSUInteger)index;
- (void) configureRoutine;
- (void) armRoutine;

// --- Draw ---
- (void) drawInMetalLayer:(nonnull CAMetalLayer*)metal_layer;

// ==== Depricated ====
//- (void) createBufferWithVertexCount:(NSUInteger)count;
//- (void) bindBuffer:(NSUInteger)index;
@end

#endif
 
