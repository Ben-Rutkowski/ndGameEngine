#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include "common.hpp"

enum t2Type { 
    t2RGBA, t2DEPTH_STENCIL
};

enum t2Filter {
    t2NEAREST
};

class Texture2D {
private:
    unsigned int texture_id;
    int width, height;

public:
    Texture2D(int width, int height);
    void config(t2Type type);
    void setFilter(t2Filter min, t2Filter max);

    unsigned int getId();

    void bind();
    void unbind();
};

#endif