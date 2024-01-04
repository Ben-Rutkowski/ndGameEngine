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
- (void) bindRoutine:(NSUInteger)index;
- (void) configureRoutine;
- (void) armRoutine;

- (void) bindBuffer:(NSUInteger)index;
- (void) createBufferWithVertexCount:(NSUInteger)count;
- (nonnull DynamicBuffer*) getBuffer;

- (void) OLDcreateBufferWithVertexCount:(NSUInteger)count;
- (nullable id<MTLBuffer>) getBufferOLD;

// --- Draw ---
- (void) drawInMetalLayer:(nonnull CAMetalLayer*)metal_layer;

@end

#endif
 
