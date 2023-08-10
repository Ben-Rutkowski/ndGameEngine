#include "event_manager.hpp"

EventCall::EventCall() 
    :function{ [](Event*)->void{} } {}

EventCall::EventCall(EventCallType func_in)
    :function{ func_in } {}

void EventCall::operator()(Event* event) {
    function(event);
}