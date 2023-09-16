#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "id_hash_table.hpp"

int main() {
    ndApp app;
    ndWindow window(800, 600, "ndGameEngine");
    app.linkWindow(&window);
    EditSpace edit_space;
    window.linkEditSpace(&edit_space);

    // app.startApp();

    IdHashTableDynamic set(5);

    set.add(4, 4);
    set.print();
    set.add(4, 12);
    set.remove(4);
    set.add(4, 11);
    set.print();
}