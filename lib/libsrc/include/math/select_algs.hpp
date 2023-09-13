#ifndef SELECT_ALGS_HPP
#define SELECT_ALGS_HPP

#include "math/matrix.hpp"

namespace sAlg {

// ======== Clip Space Algorithms ========

/* Returns true if point q crosses horizantally the line segment
from root r to end e and if the line is on the right of the line.
root and end must be in clip coords.
*/
bool crossRight(vec2 q, vec4 r, vec4 e);

/* Point must be in model space and pvm is proj*view*model.
*/
vec4 modelToClip(vec4 point, mat4 pvm);


// ======== World Space Algorithms ========

/* If the ray intersects, then return the positive length of the ray to collision.
   If the ray does not intersect, return -1.

   v1,v2,v3 are the triangle vertices, and u and d are the base and direction of the ray.
*/
float triRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d);

/* If the ray intersects, then return the positive length of the ray to collision.
   If the ray does not intersect, return -1.

   v1,v2,v3 are the parallelagram vertices, and u and d are the base and direction of the ray.
*/
float parRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d);

/* Ray at root u and direction d is tested to intersect with the plane b0 = v2 - v1 and b1 = v3 - v1.
   The first two coords of the return is the x and y components for the bases b0 and b1. The third component
   is the length of the ray at its collision.
*/
vec3 planeRayIntersect(vec4 v1, vec4 v2, vec4 v3, vec4 u, vec4 d);

}

#endif