#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

void DEBUG();

// class Event {
// private:
//     int data;

// public:
//     Event(int a) :data{a} {}
//     void debug() { std::cout << data << std::endl; }
// };


// template<typename APP>
// class EventManagerOld {
// typedef void(APP::*call)(Event*);
// private:
//     call propogateEvent;
//     APP* app_ptr;

// public:
//     EventManagerOld() {}

//     void linkApp(APP* ptr, call fun) {
//         app_ptr = ptr;
//         propogateEvent = fun;
//     }

//     void runEvent(Event* event) {
//         ((*app_ptr).*propogateEvent)(event);
//     }
// };

// class tApp {
// private:
//     EventManagerOld<tApp> manager;

// public:
//     tApp();

//     void debug();

//     void runEvent(Event* event);
// };

#endif