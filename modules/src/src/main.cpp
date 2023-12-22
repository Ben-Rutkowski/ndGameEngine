#include <iostream>
#include "cocoa_interface.hpp"

int main() {
    initCocoa();

    ndWindow mywindow(800, 600, "ndGameEngine");

    while (true) {
        pollEventsCocoa();
    }

    char quit = 'a';
    while (quit != 'q') {
        std::cin >> quit;
    }
}