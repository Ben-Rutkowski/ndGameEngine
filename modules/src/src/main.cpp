#include "application.hpp"
#include <iostream>

#include "edit_mesh.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "Hello World!");
    app.linkWindow(&window);

    app.startApp();
}