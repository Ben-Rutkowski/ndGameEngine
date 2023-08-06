/*

A Queue is a stack of objects. Users can add objects to
the end of the queue, retrive the address of the current
object and clear current object. The type of the object
must have an overloaded <!> operator. And objects created
by the empty constructor must satisfy !object

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

CONSTRUCTORS AND INITIALIZATION:
-- constructor(): Initializes every queue to null objects and sets the 
    markers to (0,0). 

GETS AND SETS:
-- getCurrent: Returns a pointer to the current object.
-- DEPRECIATED:: getEventLen: Returns the length of non-empty objects.

INTERFACE:
-- queue: Takes a reference of an object and copies it to the first empty space.
    Then increments the end point marker.
-- clearCurrent: If current object is not null, set to null and increment
    non-empty marker. Otherwise do nothing.

*/ 

#ifndef QUEUE_HPP
#define QUEUE_HPP

#define TEMP template<typename T, int N>

TEMP
class Queue
{
private:
    T cache[N];

    int current, end;

public:
    Queue();

// --- Gets and Sets ---
    T* getCurrent();

// --- Interface ---
    void queue(T object);
    void clearCurrent();
    bool isEmpty();

private:
    bool isCurrentNull();
    bool isEndNull();
};


// ======== FUNCTION DEFINITIONS ========


#include <iostream>

// === Constructors and Initialization

template<typename T,int N> Queue<T,N>::Queue(): 
current{ 0 }, end{ 0 } {}

// === Gets and Sets ===
TEMP T* Queue<T, N>::getCurrent() { return &cache[current]; }

// === Interface === 
TEMP void Queue<T,N>::queue(T object) {
    if (!isEndNull()) {
        std::cout << "CACHE IS FULL: CANNOT QUEUE EVENT!" << std::endl;
        return;
    }
    cache[end] = object;
    end = (end + 1) % N;
}

TEMP void Queue<T,N>::clearCurrent() {
    if (isCurrentNull()) {
        std::cout << "CACHE IS EMPTY: NOTHING TO CLEAR" << std::endl;
    }
    cache[current] = T();
    current = (current + 1) % N;
}

TEMP bool Queue<T,N>::isEmpty() {
    return isCurrentNull();
}

// === Private ===

TEMP bool Queue<T,N>::isCurrentNull() {
    return !(cache[current]);
}

TEMP bool Queue<T,N>::isEndNull() {
    return !(cache[end]);
}

#endif