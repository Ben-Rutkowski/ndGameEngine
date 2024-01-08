/*
    TODO: namespaces for shader_types
          buffer indices in routine and subroutines
*/

#include "application.hpp"
#include "cocoa_interface.hpp"
#include <chrono>

// #define DOES_DEBUG true
#define DOES_DEBUG false
#define DEBUG_POINT 5
#define BREAK_POINT 10
// #define DOES_BREAK true
#define DOES_BREAK false

static int count;
// static double detla;

void debugSetup() {
    count = 0;
}

void debugFirst() {
    count += 1;
    // std::cout << " \n";
    // auto start_type = std::chrono::high_resolution_clock::now();
}

void debugLast() {
    // auto end_type = std::chrono::high_resolution_clock::now();
    // auto delta_type = std::chrono::duration_cast<std::chrono::milliseconds>(end_type - start_type);
    // delta = delta_type.count();
    // std::cout << "delta: " << delta << std::endl;

    // std::cout << "\n";
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

        if (DOES_DEBUG && count%DEBUG_POINT == 0) {
            event_manager.propogateEventImmediate(
                ndEvent(module_name, Operation::DEBUG)
            );
        }

        if (DOES_BREAK && count >= BREAK_POINT) {
            break;
        }
    }
}
