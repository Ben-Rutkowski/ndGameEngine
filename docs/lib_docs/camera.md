# **class** | Camera
A camera that exists in 3D space, produces perspective and view matrices.
```
class Camera;
```

### Description
The `Camera` maintains a state in which it exists in the world. A camera has a position, and orientation. The orientation is measured by pitch, roll and yaw parameters. The camera has a field of view attribute which is used to calculate the near and far plane.

`Camera` can calculate the view matrix, perspective and orthogonal projection matrices based off of its position and orientation state. It caches this value so it only needs to be calculated once, upon an state change. It also caches the front, up and right vectors (which determine its orientation) to use for camera control or in state matrix computation.

The camera state can be set manually, or can be nudged by adding a delta to values of the position and pitch-roll-yaw. When cameras are nudged using normal camera manipulation, it's pitch is capped so it cannot look directly up or down, its yaw is computed modulus 2*pi so it is bound periodically and its roll is locked entirely.
