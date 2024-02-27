# **Draw Subroutine** | Thick Lines
Draws thick and curved lines from pairs of vertices.

### Description
Takes a buffer of pairs of `ThickLinePoint_T` vertices and draws thick and curved lines. First, uses a compute shader to trianglize the pairs of points into clusters of `INT_Point_T` vertices in `INT_Cluster_T`.

## Vertex Types
### Thick Line Point
Points that the user interact with. The `position` are 2D coordinates in screen space *i.e.* coordinates in the range (-1,1).
```
typedef struct ThickLinePoint_T {
    VECTOR_2F position;
    VECTOR_4F color;
} ThickLinePoint_T;
```

### INTERNAL Point
The vertices of the trianglized thick lines. The vector `uv` is used for semi-circle ends of the lines.
```
typedef struct INT_Point_T {
    VECTOR_2F position;
    VECTOR_2F uv;
    VECTOR_4F color;
} INT_Point_T;
```

### INTERNAL Cluster
Clusters of six `INT_Point_T` that represent a single trianglized line.
```
typedef struct INT_Cluster_T {
    INT_Point_T center[6];
    INT_Point_T tail_end[6];
    INT_Point_T tip_end[6];
} INT_Cluster_T;
```