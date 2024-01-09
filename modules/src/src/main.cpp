#include <iostream>
#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"

int main() {
    ndAppModule app;
    ndWindowModule window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    ndEditSapceModule edit_space;
    window.linkEditSpace(&edit_space);

    app.startApp();

    std::cout << "End of main()\n";
}
