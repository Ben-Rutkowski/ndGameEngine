#ifndef SELECT_ALGS_HPP
#define SELECT_ALGS_HPP

#include "math/matrix.hpp"

namespace sAlg {

/* Returns true if point q crosses horizantally the line segment
from root r to end e and if the line is on the right of the line.
root and end must be in clip coords.
*/
bool crossRight(vec2 q, vec4 r, vec4 e);

/* Point must be in model space and pvm is proj*view*model.
*/
vec4 modelToClip(vec4 point, mat4 pvm);

}

#endif