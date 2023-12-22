/* CLASS TEMPLATE: Queue
    T : type of object in queue (must have a default
        constructor and a ! operator such that objects
        created with the default constructor must return true
        from the ! operator)
    N : length of queue.

A Queue is a stack of objects. Users can add objects to
the end of the queue, retrive the address of the current
object and clear current object.

The stack is an array of objects, with a counter that marks the
first non-empty object (^) and the first empty object (o). Each stack
is essentually a recursive loop. Both counters start at (0, 0)

       o                   o                o                o
-------------    -------------    -------------    -------------
|  |xx|  |  |    |  |xx|xx|  |    |  |  |xx|  |    |  |  |  |  |    ::EMPTY::
-------------    -------------    -------------    -------------
    ^                ^                   ^                   ^

       o                   o       o                   o
-------------    -------------    -------------    -------------
|  |xx|  |  |    |  |xx|xx|  |    |  |xx|xx|xx|    |xx|xx|xx|xx|    ::FULL::
-------------    -------------    -------------    -------------
    ^                ^                ^                ^

*/ 

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <array>
#include <vector>

template<typename ElementType>
class Queue {
// --- Attributes ---
private:
    std::vector<ElementType> queue_cache;
    int current_id, end_id;

// --- Constructor ---
public:
    Queue(int size) : queue_cache(size, ElementType()), current_id{ 0 }, end_id{ 0 } {}
    ElementType* getCurrent() { return &queue_cache[current_id]; }

// --- Interface ---
public:
    void queue(ElementType element) {
        if (isEndNull()) {
            queue_cache[end_id] = element;
            stepEnd();
        } else {
            std::cout << "CACHE IS FULL: CANNOT QUEUE EVENT!" << std::endl;
        }
    }

    void queueFront(ElementType element) {
        stepCurrentBack();
        if (isCurrentNull()) {
            queue_cache[current_id] = element;
        } else {
            stepCurrent();
            std::cout << "CACHE IS FULL: CANNOT QUEUE EVENT!" << std::endl;
        }
    }

    void dequeueCurrent() {
        if (!isCurrentNull()) {
            queue_cache[current_id] = ElementType();
            stepCurrent();
        } else {
            std::cout << "CACHE IS EMPTY: NOTHING TO CLEAR" << std::endl;
        }
    }

// --- Private ---
private:
    inline bool isCurrentNull() { return !(queue_cache[current_id]); }
    inline bool isEndNull()     { return !(queue_cache[end_id]); }
    
    inline void stepCurrent()     { current_id = (current_id+1)%queue_cache.size(); }
    inline void stepEnd()         { end_id     = (end_id+1)%queue_cache.size(); }
    inline void stepCurrentBack() { current_id = (current_id+(queue_cache.size()-1))%queue_cache.size(); }

// --- Debugging ---
public:
    void debug() {
        for (int i=0; i<queue_cache.size(); i++) {
            if (i == current_id) {
                std::cout << "o";
            } else {
                std::cout << " ";
            }
            if (i == end_id) {
                std::cout << "x";
            } else {
                std::cout << " ";
            }

            std::cout << " | " << i << " | ";

            if (!queue_cache[i]) {
                std::cout << " ";
            } else  {
                std::cout << "-Full-";
            }

            std::cout << std::endl;
        }
    }
};

#endif