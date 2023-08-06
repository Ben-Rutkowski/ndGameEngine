#include "application.hpp"
#include <iostream>

void onEvent(Event* event) {
    event->print();
}

int main() {
    ndApp app;
    ndWindow window(800, 600, "Hello World!");
    app.linkWindow(&window);

    app.startApp();
}