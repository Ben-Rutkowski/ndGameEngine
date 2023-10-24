#include "rendering/texture_2d.hpp"

Texture2D::~Texture2D() { glDeleteTextures(1, &texture_id); }
Texture2D::Texture2D(int width, int height)
    :frame_width{ width }, frame_height{ height } {

    glGenTextures(1, &texture_id);
}

void Texture2D::config(t2Type type) { config(type, frame_width, frame_height); }
void Texture2D::config(t2Type type, int width, int height) {
    frame_width  = width;
    frame_height = height;

    switch (type) {
    case t2RGBA:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, NULL
        );
        return;

    case t2DEPTH_STENCIL:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
        );
        return;

    case t2DEPTH:
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
            GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL
        );
        return;

    default:
        std::cout << "NOT IMPLEMENTED" << std::endl;
    }
}

void Texture2D::setFilter(t2Filter min, t2Filter max) {
    setFilterMinMag(min, GL_TEXTURE_MIN_FILTER);
    setFilterMinMag(max, GL_TEXTURE_MAG_FILTER);
}

unsigned int Texture2D::getId() { return texture_id; } 

void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
}

void Texture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ======== Private ========
inline void Texture2D::setFilterMinMag(t2Filter type, GLenum min_mag) {
    switch (type) {
    case t2NEAREST:
        glTexParameteri(GL_TEXTURE_2D, min_mag, GL_NEAREST);
        return;
    
    default:
        std::cout << "TODO: Implement Texture2D::setFilter" << std::endl;
    }
}