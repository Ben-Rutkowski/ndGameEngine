#include "application.hpp"
#include <iostream>

#include "matrix.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "Hello World!");
    app.linkWindow(&window);

    // app.startApp();

    mat4 id = mat4::iden();
    id.print();

    vec4 vec({1.0f, 3.0f, 3.0f, 2.0f});
    mat4 mat = mat4::diag(vec);
    mat.print();

    mat4 rot = mat4::rotX(M_PI/2);
    rot.print();
    rot.roundPlace(2);
    rot.print();
}