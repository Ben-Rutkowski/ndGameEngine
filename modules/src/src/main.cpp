#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"

ndAppModule app;
ndWindowModule window(800, 600, "ndGameEngine");
ndEditSapceModule edit_space; 

int main() {
    app.linkWindow(&window);
    window.linkEditSpace(&edit_space);

    app.startApp();

    std::cout << "End of main()\n";
}
