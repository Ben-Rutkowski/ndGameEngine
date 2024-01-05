#import "DrawRoutines.h"

@implementation LineSubroutine

// ==== Configure ====
- (void) configureWithDrawablePixelFormat:(MTLPixelFormat)pixel_format {}
- (void) linkBuffer:(nonnull DynamicBuffer*)buffer {}


// ==== Resources ====
- (void) bindBuffer:(NSUInteger)index {}


// ==== Draw ====
- (void) encodeSubroutineInBuffer:(nonnull id<MTLCommandBuffer>)command_buffer
                        inTexture:(nonnull id<MTLTexture>)texture
{}

@end
