#include "application.hpp"
#include "window.hpp"
#include "edit_space.hpp"
#include <iostream>

#include "nd_module_new.hpp"

// static bool display;

// void* operator new(size_t size) {
//     if (display) {
//         // std::cout << "Allocating: " << size << " bytes\n";
//         if (size > 32000) {
//             std::cout << "Big: " << size << " bytes\n";
//         }
//     } 

//     return malloc(size);
// }

class TestApp {
private:
    EventManager event_manager;
    ndModule* test_module;

public:
    TestApp() {
        event_manager.setCallback(this, TestApp::propogateEventCallback);
    }

    void linkTestModule(ndModule* mod_ptr) {
        test_module = mod_ptr;
        test_module->setManagerPtr(&event_manager);
    }

    void propogateEvent(Event* event) {
        test_module->runEvent(event);
    }

    void debug() {
        Event event(Module::TEST, Data::DEBUG);
        propogateEvent(&event);
        event_manager.pollEvents();
    }

    static void propogateEventCallback(void* ptr, Event* event) {
        TestApp* app = (TestApp*)ptr;
        app->propogateEvent(event);
    }
};

class TestModule : public ndModuleInstance<TestModule> {
public:
    TestModule()
        :ndModuleInstance<TestModule>(Module::TEST, 4) {

        setEvent(Data::DEBUG, {&TestModule::onDebug});
        setEvent(Data::TEST, {&TestModule::onTest});
    }

    void onDebug(Event* event) {
        event->print(module_name);
        event_interface.manager->queueEvent(module_name, Data::TEST);
    }

    void onTest(Event* event) {
        event->print(module_name);
    }
};

int main() {
    // display = false;
    // ndApp app;
    // ndWindow window(800, 600, "ndGameEngine");
    // app.linkWindow(&window);
    // EditSpace edit_space;
    // window.linkEditSpace(&edit_space);

    // display = true;
    // app.startApp();

    TestApp app;
    TestModule modl;
    app.linkTestModule(&modl);
    app.debug();

}