#ifndef RENDERER_H
#define RENDERER_H

#import <MetalKit/MetalKit.h>

@interface Renderer : NSObject<MTKViewDelegate>
- (nonnull instancetype) initWithMetalKitView:(nonnull MTKView*)mtk_view;
@end

#endif
