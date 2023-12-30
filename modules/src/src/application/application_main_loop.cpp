#include "application.hpp"
#include "cocoa_interface.hpp"
#include <chrono>

void ndAppModule::startApp() {
    beginLoop();
    // pollEvents();

    int count = 0;
    double delta;
    double elapsed = 0.0;

    while (!window->requestBool(Request::SHOULD_CLOSE)) {
    // while (true) {
        count += 1;

        startFrame();
        drawWindowFrame();
        auto start_type = std::chrono::high_resolution_clock::now();
        draw();
        auto end_type = std::chrono::high_resolution_clock::now();
        endFrame();

        pollEvents();
        pollEventsCocoa();

        auto delta_type = std::chrono::duration_cast<std::chrono::milliseconds>(end_type - start_type);

        delta = delta_type.count();
        elapsed += delta;


        if (elapsed >= 3000) {
            std::cout << (float)count/3.0f << std::endl;
            count = 0;
            elapsed = 0.0;
        }

        // if (count >= 100) {
        //     break;
        // }
    }
}