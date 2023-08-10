#include <iostream>
#include "event.hpp"

// === Structs ===
EventCode::EventCode()
    :type{ EventType::null },
    module{ Module::null },
    data{ Data::null } {}

EventCode::EventCode(EventType type_in, Module module_in, Data data_in)
    :type{ type_in },
    module{ module_in },
    data{ data_in } {}

// === Constructors and Initialization ===
Event::Event() {}

Event::Event(EventCode code_in)
    :code{ code_in } {}

// === Gets and Sets ===
EventType Event::getType()   { return code.type; }
Module    Event::getModule() { return code.module; }
Data      Event::getData()   { return code.data; }
int       Event::getIndex()  { return (int)code.data; }

int       Event::getInt(int place) { return 0; }

// === Operators ===
bool Event::operator!() { return code.type == EventType::null; }

// === Debugging ===
void Event::print()
{
    std::cout << "Type: " << (int)code.type << std::endl;
    std::cout << "Module: " << (int)code.module << std::endl;
    std::cout << "Data: " << (int)code.data << std::endl;
}


// ================ EventCall ================


EventCall::EventCall() 
    :function{ [](Event*)->void{} } {}

EventCall::EventCall(EventCallType func_in)
    :function{ func_in } {}

void EventCall::operator()(Event* event) {
    function(event);
}