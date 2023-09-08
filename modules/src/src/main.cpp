#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "matrix.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    app.startApp();

    // mat4 selecti = mat4::selectPlaneIMat(
    //     vec2({-0.5f, -0.5f}),
    //     vec2({0.5f, 0.5f}),
    //     800.0f, 600.0f, 0.1f
    // );

    // vec4 test({
    //     200.0f, 150.0f, -0.1f, 1.0f
    // });

    // vec4 out = selecti*test;
    // out.print();
}