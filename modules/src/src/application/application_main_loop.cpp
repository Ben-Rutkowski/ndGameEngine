#include "application.hpp"
#include "cocoa_interface.hpp"
#include <chrono>

static int count;
static double detla;

void debugSetup() {
    count = 0;
}

void debugFirst() {
    count += 1;
    // std::cout << " ==== OPEN ====\n";
    // auto start_type = std::chrono::high_resolution_clock::now();
}

void debugLast() {
    // auto end_type = std::chrono::high_resolution_clock::now();
    // auto delta_type = std::chrono::duration_cast<std::chrono::milliseconds>(end_type - start_type);
    // delta = delta_type.count();
    // std::cout << "delta: " << delta << std::endl;

    // std::cout << " ==== CLOSE ====\n";

    // if (count >= 100) {
    //     break;
    // }
}

void ndAppModule::startApp() {
    beginStartUp();
    
    // --- Debug ---
    debugSetup();
    // --- Debug ---

    endStartUp();
    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        // --- Debug ---
        debugFirst();
        // --- Debug ---

        startFrame();
        drawWindowFrame();
        draw();
        endFrame();

        pollEvents();
        pollEventsCocoa();

        // --- Debug ---
        debugLast();
        // --- Debug ---
    }
}
