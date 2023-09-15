#include "new_hash_table.hpp"
#include <iostream>
#include <numeric>

IdHashTable::IdHashTable(uint n)
    :capacity{ n },
    open_marker{ 0 },
    next_empty_open_marker{ 0 } {

    helper.resize(capacity);
    std::fill_n(helper.begin(), capacity, 0);
    
    data.resize(capacity + 1);

    open_spaces.resize(capacity);
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

void IdHashTable::add(uint key, uint value) {
    if (isTableFull()) {
        std::cout << "Table is Full, TODO: add resize function" << std::endl;
        return;
    }
    uint place;
    uint hash = hashFunction(key);
    if (isHelperEmpty(hash)) {
        place = linkHelper(hash);
        startThread(place, key, value);
    } else {
        place = top(hash);
        place = searchThreadToBottom(place, key);
        if (place != 0) {
            addToThread(place, key, value);
        } else {
            std::cout << "Added Redundant Key" << std::endl;
            return;
        }
    }
}

void IdHashTable::remove(uint key) {
    if (isTableEmpty()) {
        std::cout << "Tried to Remove an element on an empty table" << std::endl;
    }

    uint hash = hashFunction(key);
    uint place;
    if (!isHelperEmpty(hash)) {
        place = top(hash);
        place = searchThreadFor(place, key);
        if (place != 0) {
            removeFromThread(hash, place);
        } else {
            std::cout << "Removed a Key that didn't Exist" << std::endl;
            return;
        }
    }
}

uint IdHashTable::operator[](int i) { return data[i+1].key; }
uint IdHashTable::value(uint key) {
    uint place = top(hashFunction(key));
    place = searchThreadFor(place, key);
    if (place != 0 ) {
        return data[place].value;
    } else {
        return 0;
    }
}
bool IdHashTable::isElement(uint key) {
    uint place = top(hashFunction(key));
    place = searchThreadFor(place, key);
    return place != 0;
}

int IdHashTable::size()   { return capacity; }
void IdHashTable::clear() {
    open_marker = 0;
    next_empty_open_marker = 0;

    // helper.resize(capacity);
    std::fill_n(helper.begin(), capacity, 0);
    
    // data.resize(capacity + 1);
    std::fill_n(data.begin(), capacity + 1, HTNode());

    // open_spaces.resize(capacity);
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

void IdHashTable::resizeClear(int n) {
    capacity = n;
    open_marker = 0;
    next_empty_open_marker = 0;

    helper.resize(capacity);
    std::fill_n(helper.begin(), capacity, 0);
    
    data.resize(capacity + 1);
    std::fill_n(data.begin(), capacity + 1, HTNode());

    open_spaces.resize(capacity);
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

// === Private ===
uint IdHashTable::hashFunction(uint key) {
    key = key * 37;
    return key % capacity;
}

bool IdHashTable::isTableFull() { return open_spaces[open_marker] == 0; }
bool IdHashTable::isTableEmpty() { return open_spaces[next_empty_open_marker] != 0; }

// Helper
uint IdHashTable::linkHelper(uint hash) {
    uint next_open = getNextOpen();
    advanceNextOpen();

    helper[hash] = next_open;
    return next_open;
}

bool IdHashTable::isHelperEmpty(uint hash) { return helper[hash] == 0; }

// Thread
uint IdHashTable::next(uint place) { return data[place].next; }
uint IdHashTable::prev(uint place) { return data[place].previous; }
uint IdHashTable::top(uint hash)   { return helper[hash]; }

void IdHashTable::startThread(uint thread_i, uint key, uint value) {
    data[thread_i].key      = key;
    data[thread_i].value    = value;
    data[thread_i].previous = 0;
    data[thread_i].next     = 0;
}

void IdHashTable::addToThread(uint bottom, uint key, uint value) {
    uint next_open = getNextOpen();
    advanceNextOpen();

    data[bottom].next = next_open;

    data[next_open].key      = key;
    data[next_open].value    = value;
    data[next_open].previous = bottom;
    data[next_open].next     = 0;
}

void IdHashTable::removeFromThread(uint hash, uint place) {
    freeNewOpen(place);

    if (isTopThread(place)) {
        joinThreadTop(hash, place);
    } else {
        joinThreadNotTop(place);
    }

    data[place] = HTNode();
}

void IdHashTable::joinThreadTop(uint hash, uint place) {
    uint next_place;
    if (!isBottomThread(place)) {
        next_place = next(place);
        helper[hash] = next_place;
        data[next_place].previous = 0;
    } else {
        helper[hash] = 0;
    }
}

void IdHashTable::joinThreadNotTop(uint place) {
    uint next_place;
    uint prev_place = prev(place);
    if (!isBottomThread(place)) {
        next_place = next(place);
        data[prev_place].next = next_place;
        data[next_place].previous = prev_place;
    } else {
        data[prev_place].next = 0;
    }
}

uint IdHashTable::searchThreadToBottom(uint place, uint key) {
    while (true) {
        if (isSameKey(place, key)) {
            return 0;
        } else if (isBottomThread(place)) {
            break;
        } else {
            place = next(place);
        }
    }
    return place;
}

uint IdHashTable::searchThreadFor(uint place, uint key) {
    while (true) {
        if (isSameKey(place, key)) {
            return place;
        } else if (isBottomThread(place)) {
            break;
        } else {
            place = next(place);
        }
    }
    return 0;
}

bool IdHashTable::isTopThread(uint thread_i)    { return prev(thread_i) == 0; }
bool IdHashTable::isBottomThread(uint thread_i) { return next(thread_i) == 0; }
bool IdHashTable::isSameKey(uint thread_i, uint key_compare) { return data[thread_i].key == key_compare; }

// Open
uint IdHashTable::getNextOpen() { return open_spaces[open_marker]; }
void IdHashTable::advanceNextOpen() {
    open_spaces[open_marker] = 0;
    open_marker = (open_marker + 1) % capacity;
}

void IdHashTable::freeNewOpen(uint place) {
    open_spaces[next_empty_open_marker] = place;
    next_empty_open_marker = (next_empty_open_marker + 1) % capacity;
}

// === Debugging ===
void IdHashTable::printHelper() {
    std::cout << "Helper: " << std::endl;
    for (int i=0; i<capacity; i++) {
        std::cout << helper[i] << std::endl;
    } 
}

void IdHashTable::printData() {
    std::cout << "Data: " << std::endl;

    for (int i=0; i<capacity+1; i++) {
        data[i].print(i);
    }
}

void IdHashTable::printOpen() {
    std::cout << "Open Spaces: " << std::endl;
    for (int i=0; i<capacity; i++) {
        if (i == open_marker) {
            std::cout << "+";
        } else {
            std::cout << " ";
        }
        if (i == next_empty_open_marker) {
            std::cout << "-";
        } else {
            std::cout << " ";
        }
        std::cout << open_spaces[i] << std::endl;
    }
}

void IdHashTable::print() {
    printHelper();
    std::cout << std::endl;
    printData();
    std::cout << std::endl;
    printOpen();
    std::cout << std::endl;
}