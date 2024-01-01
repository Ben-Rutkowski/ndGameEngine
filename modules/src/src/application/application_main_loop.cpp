#include "application.hpp"
#include "cocoa_interface.hpp"
#include <chrono>

void ndAppModule::startApp() {
    beginStartUp();
    
    // pollEvents();
    // int count = 0;
    // double delta;
    // double elapsed = 0.0;

    endStartUp();
    while (!window->requestBool(Request::SHOULD_CLOSE)) {
        // count += 1;
        // auto start_type = std::chrono::high_resolution_clock::now();

        startFrame();
        drawWindowFrame();
        draw();
        endFrame();

        pollEvents();
        pollEventsCocoa();

        // auto end_type = std::chrono::high_resolution_clock::now();
        // auto delta_type = std::chrono::duration_cast<std::chrono::milliseconds>(end_type - start_type);
        // delta = delta_type.count();
        // elapsed += delta;
        // std::cout << "delta: " << delta << std::endl;
        // if (elapsed >= 1000) {
        //     std::cout << (float)count/1.0f << std::endl;
        //     count = 0;
        //     elapsed = 0.0;
        // }
        // if (count >= 2) {
        //     break;
        // }
    }
}