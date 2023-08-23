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

======== ATTRIBUTES ========
- array<T,N> cache
- int        current, end


======== METHODS ========
-------- CONSTRUCTOR --------
- Queue : Sets the current and end markers to 0.

-------- GETS AND SETS --------
- getCurrent : Returns a pointer to the current object.
    ==Return==
        - T*

-------- CONTROLS --------
- queue : Takes an object and copies it to the next open slot.
    Then increments the end point marker.
    ==Parameter==
        - T object

- clearCurrent : If current object is not null, set to null and
    increment non-empty marker. Otherwise do nothing.

-------- PRIVATE --------
- isCurrentNull : Tests if queue is empty. Returns true if the
    object in the current slot is null (i.e. !object is true).
    ==Return==
        - bool

- isEndNull : Tests if queue is full. Returns true if the
    object in the end slot is null
    ==Return==

*/ 

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <functional>
#include <array>

#define TEMP template<typename T, int N>

template<typename T, int N>
class Queue {
// --- Attributes ---
private:
    std::array<T,N> cache;
    int current, end;

// --- Constructor ---
public:
    Queue() :current{ 0 }, end{ 0 } {}

// --- Gets and Sets ---
public:
    T* getCurrent() { return &cache[current]; }

// --- controls ---
public:
    void queue(T object) {
        if (!isEndNull()) {
            std::cout << "CACHE IS FULL: CANNOT QUEUE EVENT!" << std::endl;
            return;
        }
        cache[end] = object;
        stepEnd();
    }

    void clearCurrent() {
        if (isCurrentNull()) {
            std::cout << "CACHE IS EMPTY: NOTHING TO CLEAR" << std::endl;
        }
        cache[current] = T();
        stepCurrent();
    }

    bool isEmpty() { return isCurrentNull(); }

// --- Private ---
private:
    bool isCurrentNull() { return !(cache[current]); }
    bool isEndNull()     { return !(cache[end]); }

    void stepCurrent() { current = (current + 1) % N; }
    void stepEnd()     { end     = (end + 1) % N; }
};

#endif