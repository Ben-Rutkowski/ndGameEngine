#include <iostream>
#include "window.hpp"

void ndWindow::collectKeys() {
    if (isKeyPress(GLFW_KEY_ESCAPE)) {
        queueEvent(EventType::ACTION, Data::ESCAPE_KEY);
    }
}

void ndWindow::runEventWindow(Event* event) {
    event_interface(event);
}

// === Private ===
void ndWindow::setEventCalls() {
    event_interface.setCallback(Data::ESCAPE_KEY, PACK(ndWindow::onEscapeKey));
    event_interface.setCallback(Data::CLOSE_APP, PACK(ndWindow::onCloseApp));
}

void ndWindow::onEscapeKey(Event* event) {
    queueEvent(EventType::ACTION, Data::CLOSE_APP);
}

void ndWindow::onCloseApp(Event* event) {
    setShouldClose(true);
}

void ndWindow::queueEvent(EventType type, Data data) {
    event_interface.queueEvent(EventCode{
        type, mod, data
    });
}