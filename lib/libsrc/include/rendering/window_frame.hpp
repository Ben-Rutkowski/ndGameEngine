#ifndef FRAME_HPP
#define FRAME_HPP

#include "rendering/texture_2d.hpp"
#include "vertex_buffer_interface.hpp"

/*  CLASS: wFrameBase 
    A Frame is a framebuffer with texture that can be rendered to.
    The texture can be resized. The child classes of wFrameBase specify
    what dimensions and attatchments the frame has.
*/

struct wFrameVert {
    vec2 position;
    vec2 tex_coord;
};

class wFrameBase {
protected:
    unsigned int fbo;
    int frame_width, frame_height;

// --- Rendering ---
    VertexBufferInterface vbi;
    wFrameVert   vertices[4];
    unsigned int indices[6];

public:
    wFrameBase(vec2 root, vec2 end, int width, int height);
    ~wFrameBase();

    void load();
    virtual void draw(ShaderProgram& program) = 0;
    // virtual void resize(int width, int height) = 0;

    void startDraw();
    void endDraw();
    void bind();     // Draw and Read
    void bindDraw(); // Draw
    void bindRead(); // Read
    void unbind();

private:
    void setVerts(vec2 root, vec2 end);
    void setIndices();
};

/*  CLASS: wFrameCDS
    A frame with Color, Depth, and Stencil Buffers */

class wFrameCDS : public wFrameBase {
private:
    Texture2D color_texture;
    Texture2D depth_stencil_texture;

public:
    wFrameCDS(vec2 root, vec2 end, int width, int height);
    ~wFrameCDS();

    void draw(ShaderProgram& program);
    void bindColorTexture();
};

#endif