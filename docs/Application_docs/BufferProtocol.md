# **Protocol** | BufferProtocol
A virtual buffer that keeps a rotation of buffers. Allows to parallelize draw encoding and writing.
```
@protocol BufferProtocol
```

### Description
A virtual buffer that abstracts the `MTLBuffer` protocol and allows for optimization depending on the use. Usually keeps a revolving set of `id<MTLBuffer>`, that allows one buffer to be drawn from and another to be written to. Each buffer contains a static vertex type that is set when creating a buffer.

### Usage

At the beginning of a draw call, you must call `predrawOpen` to open the buffer to be read. During any encoding, use `drawTap` to get the `id<MTLBuffer>` of the buffer. After encoding the entire draw call, call `predrawClosed`.

When writing to the buffer, call `writeOpen` to receive the `id<MTLBuffer>`. Once finished writing, call `id<writeClose>`.

When a buffer is open to draw it cannot be written from; when it is open to write, it cannot be encoded to draw from.

# Drawing

## Method | getVertexCount
Gets number of vertices that the virtual buffer holds.
```
- (NSUInteger) getVertexCount;
```

## **Method** | drawTap
Gets the `id<MTLBuffer>` used to encode draw calls; buffer must be opened to draw first.
```
- (nullable id<MTLBuffer>) drawTap;
```
**Returns**: the `id<MTLBuffer>` of the most recent buffer if opened to draw; will return `nil` otherwise.

## **Method** | drawRelay
Force gets the first `id<MTLBuffer>`.
```
- (nullable id<MTLBuffer>) drawRelay;
```
**Returns**: the first `id<MTLBuffer>` of the virtual buffer.

## **Method** | predrawOpen
Opens the buffer up to encode draw calls; must be called before tapping using `drawTap`.
```
- (void) predrawOpen;
```

## **Method** | predrawClose
Closes the buffer to any more draw calls.
```
- (void) predrawClose;
```

## **Method** | drawCompleted
Is called in a separate thread when the GPU has finished computing its frame. Important for internal semaphore racing.
```
- (void) drawCompleted;
```

# Writing
## **Method** | writeOpen
Opens the buffer to write to and gets the `id<MTLBuffer>` to write to.
```
- (nullable id<MTLBuffer>) writeOpen;
```
**Returns**: a `id<MTLBuffer>` to write to.

## **Method** | writeClose
Closes the buffer from writing so that it can be drawn from.
```
- (void) writeClose;
```
