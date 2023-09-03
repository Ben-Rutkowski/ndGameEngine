#include "application.hpp"
#include <iostream>

int main() {
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    app.startApp();
}