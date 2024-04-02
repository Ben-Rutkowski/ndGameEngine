# **Class** | Edit Space
An ndModule for editing 3D meshes.
```
class ndEditSpaceModule : public ndModuleImplement<ndEditSpaceStateLen>;
```

### Description
Contains the draw routine interface and sets the vertex data to be rendered. The module is in charge of managing three different buffers; the point buffer which has a non redundant amount of points, the edge buffer which has redundant points, each for a specific line, and the tri buffer which has many redundant buffers. These buffers live in the Cocao side of the application and `ndEditSpaceModule` communicates with this using the interface.

# Methods
## Buffer Operations
Adds an array of points to the buffer, and returns the starting index of the chunk of points
### Add Points
```
uint32_t addPoints(const vec4* points, const int len);
```
`const vec4* points`: a pointer to an array of position for the points to be added
`const int len`: the length of the array
Return `uint32_t`: the index in the buffer pointing to the beggining of the new chunk of points

### Delete Point
Replaces the point at the given index with the last element of the buffer, and deletes the last element. Returns the index of where the last element in the buffer used to be.
```
uint32_t deletePoint(const uint32_t d_point);
```
`const uint32_t d_point`: the index of the point that is to be replaced
Return `uint32_t`: the index of the last point that was copied and deleted.
