#include <iostream>
#include "application.hpp"
#include "window.hpp"

int main() {
    ndAppModule app;
    ndWindowModule window(800, 600, "ndGameEngine");
    app.linkWindow(&window);

    app.startApp();

    std::cout << "End of main()\n";
}