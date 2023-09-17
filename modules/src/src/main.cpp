#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "id_hash_table.hpp"

static bool display;

void* operator new(size_t size) {
    if (display) {
        // std::cout << "Allocating: " << size << " bytes\n";
        if (size > 32000) {
            std::cout << "Big: " << size << " bytes\n";
        }
    } 

    return malloc(size);
}

int main() {
    display = false;
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    display = true;
    app.startApp();
}