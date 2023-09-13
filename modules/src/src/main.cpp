#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "math/matrix.hpp"
#include "math/select_algs.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    app.startApp();
}