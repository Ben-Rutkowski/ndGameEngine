# **Class** | Edit Mesh
Contains and manages all DCEL objects that correspond to a single 3D mesh.
```
class EditMesh;
```

### Description 

# **Struct** | DCEL Half Edge
A one directional edge in the face of a mesh.
```
struct DCEL_HalfEdge;
```
Half edge points from one root point to a tip point. It's twin half edge points between the same two points but in the opposite direction. Each face is represented by a directed cycle of half edges on the inside of the face. Faces are oriented counter-clock-wise.

## Attributes
`uint32_t vertex`: index of the point in the point buffer of the root of the edge.

`uint32_t next_hedge`: index of the next half edge in the face cycle, 0 if half edge is not part of face.

`uint32_t prev_hedge`: index of the previous half edge in the face cycle, 0 if half edge is not part of face.

`uint32_t twin_hedge`: index of the twin half edge.

## **Struct** | DCEL Face
A face comprised of a counter-clock-wise directed cycle of inner half edges.
```
struct DCEL_Face;
```

## Attributes
`uint32_t inner_hedge`: the index of one half edge that is part of the inner half edge cycle of the face
`uint32_t size`: the number of half edges in the inner half edge cycle

