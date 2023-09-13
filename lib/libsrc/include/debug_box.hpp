#ifndef DEBUG_BOX_HPP
#define DEBUG_BOX_HPP

#include "rendering/vertex_buffer_interface.hpp"

#define DEBUG_SHADER_SUB_DIR "modules/src/shaders/debugging/"

class DebugBox {
private:
    vec4 vertex_data[4];
    VertexBufferInterface vbi;
    ShaderProgram program;

public:
    DebugBox();

    void setVerts(vec4 v1, vec4 v2, vec4 v3, vec4 v4);

    void draw(mat4 view, mat4 proj);
};

#endif