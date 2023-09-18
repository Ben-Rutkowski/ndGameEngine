// #include "event_manager.hpp"

// // === Constructors and Initialization ===
// EventManagerOld::EventManagerOld() {}

// void EventManagerOld::setCallback(EventCallOld event_call) {
//     propogateEventCall = event_call;
// }

// // === Interface ===
// void EventManagerOld::queueEvent(Module module_name, Data data) {
//     event_queue.queue(Event(module_name, data));
// }

// void EventManagerOld::queueEvent2i(Module module_name, Data data, vec2i vector) {
//     event2i_queue.queue(Event2i(module_name, data, vector));
// }

// void EventManagerOld::queueEvent2f(Module module_name, Data data, vec2 vector) {
//     event2f_queue.queue(Event2f(module_name, data, vector));
// }

// void EventManagerOld::queueEvent4f(Module module_name, Data data, vec4 vector) {
//     event4f_queue.queue(Event4f(module_name, data, vector));
// }

// void EventManagerOld::propogateEvent(Event* event) {
//     propogateEventCall(event);
// }

// void EventManagerOld::pollEvents() {
//     Event* current_event;

//     while (!event_queue.isEmpty()) {
//         current_event = event_queue.getCurrent();
//         propogateEventCall(current_event);
//         event_queue.clearCurrent();
//     }

//     while (!event2i_queue.isEmpty()) {
//         current_event = event2i_queue.getCurrent();
//         propogateEventCall(current_event);
//         event2i_queue.clearCurrent();
//     }

//     while (!event2f_queue.isEmpty()) {
//         current_event = event2f_queue.getCurrent();
//         propogateEventCall(current_event);
//         event2f_queue.clearCurrent();
//     }
// }