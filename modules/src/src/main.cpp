#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "new_hash_table.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    app.startApp();
}