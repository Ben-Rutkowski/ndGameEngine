#include "application.hpp"
#include <iostream>
#include "matrix.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "Hello World!");
    app.linkWindow(&window);

    app.startApp();
}