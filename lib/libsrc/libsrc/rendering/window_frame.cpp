#include "rendering/window_frame.hpp"

wFrameBase::wFrameBase(vec2 root, vec2 end, int width, int height)
    :frame_width{ width },
    frame_height{ height },
    root{ root },
    end{ end } {
    glGenFramebuffers(1, &fbo);

    setVerts(root, end);
    setIndices();
}
wFrameBase::~wFrameBase() { glDeleteFramebuffers(1, &fbo); }

void wFrameBase::resizeRelative(int global_width, int global_height) {
    std::cout << "Global Width: " << global_width << ", Global Height: " << global_height << std::endl;

    int width   = math::clipToPixel(end[0], global_width) - math::clipToPixel(root[0], global_width);
    int height  = math::clipToPixel(end[1], global_height) - math::clipToPixel(root[1], global_height);

    std::cout << "Width: " << width << ", Height: " << height << std::endl;
    resize(width, height);
}

void wFrameBase::load() {
    vbi.bindAllBuffers();
    vbi.loadVerticesStatic(vertices, sizeof(vertices));
    vbi.loadIndicesStatic(indices, sizeof(indices));
    vbi.configAttribf(0, 2, sizeof(wFrameVert), (void*)0);
    vbi.configAttribf(1, 2, sizeof(wFrameVert), (void*)(sizeof(vec2)));
    vbi.unbindCurrent();
}

void wFrameBase::startDraw() {
    glViewport(0, 0, frame_width, frame_height);
    bindDraw();
}

void wFrameBase::endDraw() {
    unbind();
}

void wFrameBase::bind()     { glBindFramebuffer(GL_FRAMEBUFFER, fbo); }
void wFrameBase::bindDraw() { glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo); }
void wFrameBase::bindRead() { glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); }
void wFrameBase::unbind()   { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

// === Private ===
void wFrameBase::setVerts(vec2 root, vec2 end) {
    vertices[0] = { root, vec2(0.0f) };                        // Low Left
    vertices[1] = { vec2({end[0], root[1]}), vec2::basis(0) }; // Low Right
    vertices[2] = { end, vec2(1) };                            // Top Right
    vertices[3] = { vec2({root[0], end[1]}), vec2::basis(1) }; // Top Left
}

void wFrameBase::setIndices() {
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;
}

// ======== wFrameCDS ========
wFrameCDS::~wFrameCDS() {}
wFrameCDS::wFrameCDS(vec2 root, vec2 end, int width, int height)
    :wFrameBase(root, end, width, height),
    color_texture(width, height),
    depth_stencil_texture(width, height) {

    color_texture.bind();
    color_texture.config(t2RGBA);
    color_texture.setFilter(t2NEAREST, t2NEAREST);
    depth_stencil_texture.bind();
    depth_stencil_texture.config(t2DEPTH_STENCIL);
    depth_stencil_texture.setFilter(t2NEAREST, t2NEAREST);
    depth_stencil_texture.unbind();

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture.getId(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture.getId(), 0);
    unbind();
}

void wFrameCDS::draw(ShaderProgram& program) {
    glActiveTexture(GL_TEXTURE0);
    color_texture.bind();

    program.use();
    program.uniform1i("frame_texture", 0);

    vbi.bindCurrent();
    vbi.drawElementsTriangles(6);
    vbi.unbindCurrent();
}

void wFrameCDS::resize(int width, int height) {
    frame_width  = width;
    frame_height = height;

    color_texture.bind();
    color_texture.config(t2RGBA, frame_width, frame_height);
    depth_stencil_texture.bind();
    depth_stencil_texture.config(t2DEPTH_STENCIL, frame_width, frame_height);
    depth_stencil_texture.unbind();
}

// ======== wFrameD ========
wFrameD::~wFrameD() {}
wFrameD::wFrameD(vec2 root, vec2 end, int width, int height)
    :wFrameBase(root, end, width, height),
    depth_texture(width, height) {

    depth_texture.bind();
    depth_texture.config(t2DEPTH);
    depth_texture.setFilter(t2NEAREST, t2NEAREST);
    depth_texture.unbind();

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture.getId(), 0);
    unbind();
}

void wFrameD::draw(ShaderProgram& program) { std::cout << "wFrameD::draw not implemented" << std::endl; }

void wFrameD::resize(int width, int height) {
    frame_width  = width;
    frame_height = height;

    depth_texture.bind();
    depth_texture.config(t2DEPTH, frame_width, frame_height);
    depth_texture.unbind();
}

unsigned int wFrameD::getID() {
    return depth_texture.getId();
}