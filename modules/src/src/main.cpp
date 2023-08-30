#include "application.hpp"
#include <iostream>

#include "matrix.hpp"
#include <math.h>

int main() {
    ndApp app;
    ndWindow window(800, 600, "Hello World!");
    app.linkWindow(&window);

    app.startApp();
}