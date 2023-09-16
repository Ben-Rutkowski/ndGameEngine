#include "id_hash_table.hpp"
#include <iostream>
#include <numeric>

// ================ IdHashTableBase ================

IdHashTableBase::IdHashTableBase()
    :IdHashTableBase(0) {}

IdHashTableBase::IdHashTableBase(uint n)
    :capacity_size{ n },
    open_marker{ 0 } {

    helper.resize(capacity_size);
    std::fill_n(helper.begin(), capacity_size, 0);
    
    data.resize(capacity_size + 1);

    open_spaces.resize(capacity_size);
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

// === Hash ===
bool IdHashTableBase::isTableFull() { return open_spaces[open_marker] == 0; }
uint IdHashTableBase::hashFunction(uint key) {
    key = key * 37;
    return key % capacity_size;
}

// === Interface ===
void IdHashTableBase::add(uint key) { add(key, 0); }
void IdHashTableBase::add(uint key, uint value) {
    // if (isTableFull()) {
    //     std::cout << "Table is Full!" << std::endl;
    //     return;
    // }
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
            // std::cout << "Added Redundant Key!" << std::endl;
            return;
        }
    }
}

uint IdHashTableBase::value(uint key) {
    uint place = top(hashFunction(key));
    place = searchThreadForKey(place, key);
    if (place != 0 ) {
        return data[place].value - 1;
    } else {
        return 0;
    }
}
bool IdHashTableBase::isElement(uint key) {
    uint place = top(hashFunction(key));
    place = searchThreadForKey(place, key);
    return place != 0;
}

int  IdHashTableBase::capacity()  { return capacity_size; }
void IdHashTableBase::clear() {
    open_marker = 0;

    std::fill_n(helper.begin(), capacity_size, 0);    
    std::fill_n(data.begin(), capacity_size + 1, HTNode());
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

void IdHashTableBase::resizeClear(int n) {
    capacity_size = n;
    open_marker = 0;

    helper.resize(capacity_size);
    data.resize(capacity_size + 1);
    open_spaces.resize(capacity_size);

    std::fill_n(helper.begin(), capacity_size, 0);
    std::fill_n(data.begin(), capacity_size + 1, HTNode());
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

// === Helper ===
bool IdHashTableBase::isHelperEmpty(uint hash) { return helper[hash] == 0; }
uint IdHashTableBase::linkHelper(uint hash) {
    uint next_open = getNextOpen();
    stepNextOpen();

    helper[hash] = next_open;
    return next_open;
}

// === Thread ===
uint IdHashTableBase::next(uint place) { return data[place].next; }
uint IdHashTableBase::prev(uint place) { return data[place].previous; }
uint IdHashTableBase::top(uint hash)   { return helper[hash]; }

void IdHashTableBase::startThread(uint thread_i, uint key, uint value) {
    data[thread_i].key      = key;
    data[thread_i].value    = value + 1;
    data[thread_i].previous = 0;
    data[thread_i].next     = 0;
}

void IdHashTableBase::addToThread(uint bottom, uint key, uint value) {
    uint next_open = getNextOpen();
    stepNextOpen();

    data[bottom].next = next_open;

    data[next_open].key      = key;
    data[next_open].value    = value + 1;
    data[next_open].previous = bottom;
    data[next_open].next     = 0;
}

uint IdHashTableBase::searchThreadToBottom(uint place, uint key) {
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

uint IdHashTableBase::searchThreadForKey(uint place, uint key) {
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

bool IdHashTableBase::isTopThread(uint thread_i)    { return prev(thread_i) == 0; }
bool IdHashTableBase::isBottomThread(uint thread_i) { return next(thread_i) == 0; }
bool IdHashTableBase::isSameKey(uint thread_i, uint key_compare) { return data[thread_i].key == key_compare; }

// === Open Spaces ===
uint IdHashTableBase::getNextOpen() {
    return open_spaces[open_marker];
}

void IdHashTableBase::stepNextOpen() {
    open_spaces[open_marker] = 0;
    open_marker = (open_marker + 1) % capacity_size;
}

// === Debugging ===
void IdHashTableBase::printHelper() {
    std::cout << "Helper: " << std::endl;
    for (int i=0; i<capacity_size; i++) {
        std::cout << helper[i] << std::endl;
    } 
}

void IdHashTableBase::printData() {
    std::cout << "Data: " << std::endl;

    for (int i=0; i<capacity_size+1; i++) {
        data[i].print(i);
    }
}

void IdHashTableBase::printOpen() {
    std::cout << "Open Spaces: " << std::endl;
    for (int i=0; i<capacity_size; i++) {
        if (i == open_marker) {
            std::cout << "+";
        } else {
            std::cout << " ";
        }
        
        std::cout << open_spaces[i] << std::endl;
    }
}

void IdHashTableBase::print() {
    printHelper();
    std::cout << std::endl;
    printData();
    std::cout << std::endl;
    printOpen();
    std::cout << std::endl;
}

// ================ IdHashTableStatic ================
IdHashTableStatic::IdHashTableStatic()
    :IdHashTableBase() {}

IdHashTableStatic::IdHashTableStatic(uint n)
    :IdHashTableBase(n) {}

int IdHashTableStatic::size() {
    if (open_marker == 0 && open_spaces[0] == 0) {
        return capacity_size;
    } else {
        return open_marker;
    }
}
uint IdHashTableStatic::operator[](int i) { return data[i+1].key; }

// ================ IdHashTableDynamic ================
IdHashTableDynamic::IdHashTableDynamic()
    :IdHashTableBase(0),
    next_empty_open_marker{0} {}

IdHashTableDynamic::IdHashTableDynamic(uint n)
    :IdHashTableBase(n),
    next_empty_open_marker{0} {}

bool IdHashTableDynamic::remove(uint key) {
    uint hash = hashFunction(key);
    uint place;
    if (!isHelperEmpty(hash)) {
        place = top(hash);
        place = searchThreadForKey(place, key);
        if (place != 0) {
            removeFromThread(hash, place);
            return true;
        }
    }
    std::cout << "Removed a Key that didn't Exist" << std::endl;
    return false;
}

bool IdHashTableDynamic::canForceKey()      { return isTableFull(); }
uint IdHashTableDynamic::forceGetKey(int i) { return data[i+1].key; }

// === Private ===
void IdHashTableDynamic::removeFromThread(uint hash, uint place) {
    freeNewOpen(place);

    if (isTopThread(place)) {
        joinThreadTop(hash, place);
    } else {
        joinThreadNotTop(place);
    }

    data[place] = HTNode();
}

void IdHashTableDynamic::joinThreadTop(uint hash, uint place) {
    uint next_place;
    if (!isBottomThread(place)) {
        next_place = next(place);
        helper[hash] = next_place;
        data[next_place].previous = 0;
    } else {
        helper[hash] = 0;
    }
}

void IdHashTableDynamic::joinThreadNotTop(uint place) {
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

void IdHashTableDynamic::freeNewOpen(uint place) {
    open_spaces[next_empty_open_marker] = place;
    next_empty_open_marker = (next_empty_open_marker + 1) % capacity_size;
}


// ==================

// IdHashTableRemovable::IdHashTableRemovable()
//     :IdHashTableRemovable(0) {}

// IdHashTableRemovable::IdHashTableRemovable(uint n)
//     :capacity{ n },
//     open_marker{ 0 },
//     next_empty_open_marker{ 0 } {

//     helper.resize(capacity);
//     std::fill_n(helper.begin(), capacity, 0);
    
//     data.resize(capacity + 1);

//     open_spaces.resize(capacity);
//     std::iota(open_spaces.begin(), open_spaces.end(), 1);
// }

// void IdHashTableRemovable::add(uint key) { add(key, 0); }
// void IdHashTableRemovable::add(uint key, uint value) {
//     if (isTableFull()) {
//         std::cout << "Table is Full, TODO: add resize function" << std::endl;
//         return;
//     }
//     uint place;
//     uint hash = hashFunction(key);
//     if (isHelperEmpty(hash)) {
//         place = linkHelper(hash);
//         startThread(place, key, value);
//     } else {
//         place = top(hash);
//         place = searchThreadToBottom(place, key);
//         if (place != 0) {
//             addToThread(place, key, value);
//         } else {
//             std::cout << "Added Redundant Key" << std::endl;
//             return;
//         }
//     }
// }

// bool IdHashTableRemovable::remove(uint key) {
//     if (isTableEmpty()) {
//         std::cout << "Tried to Remove an element on an empty table" << std::endl;
//         return false;
//     }

//     uint hash = hashFunction(key);
//     uint place;
//     if (!isHelperEmpty(hash)) {
//         place = top(hash);
//         place = searchThreadFor(place, key);
//         if (place != 0) {
//             removeFromThread(hash, place);
//             return true;
//         } else {
//             std::cout << "Removed a Key that didn't Exist" << std::endl;
//             return false;
//         }
//     }
// }

// uint IdHashTableRemovable::operator[](int i) { 
//     if (!isTableFull()) {
//         std::cout << "ILLEGAL SET INTERABLE: set is not full" << std::endl;
//         return 0;
//     }
//     return data[i+1].key;
// }
// uint IdHashTableRemovable::value(uint key) {
//     uint place = top(hashFunction(key));
//     place = searchThreadFor(place, key);
//     if (place != 0 ) {
//         return data[place].value;
//     } else {
//         return 0;
//     }
// }
// bool IdHashTableRemovable::isElement(uint key) {
//     uint place = top(hashFunction(key));
//     place = searchThreadFor(place, key);
//     return place != 0;
// }

// int IdHashTableRemovable::size()   { return capacity; }
// void IdHashTableRemovable::clear() {
//     open_marker = 0;
//     next_empty_open_marker = 0;

//     // helper.resize(capacity);
//     std::fill_n(helper.begin(), capacity, 0);
    
//     // data.resize(capacity + 1);
//     std::fill_n(data.begin(), capacity + 1, HTNode());

//     // open_spaces.resize(capacity);
//     std::iota(open_spaces.begin(), open_spaces.end(), 1);
// }

// void IdHashTableRemovable::resizeClear(int n) {
//     capacity = n;
//     open_marker = 0;
//     next_empty_open_marker = 0;

//     helper.resize(capacity);
//     std::fill_n(helper.begin(), capacity, 0);
    
//     data.resize(capacity + 1);
//     std::fill_n(data.begin(), capacity + 1, HTNode());

//     open_spaces.resize(capacity);
//     std::iota(open_spaces.begin(), open_spaces.end(), 1);
// }

// // === Private ===
// uint IdHashTableRemovable::hashFunction(uint key) {
//     key = key * 37;
//     return key % capacity;
// }

// bool IdHashTableRemovable::isTableFull() { return open_spaces[open_marker] == 0; }
// bool IdHashTableRemovable::isTableEmpty() { return open_spaces[next_empty_open_marker] != 0; }

// // Helper
// uint IdHashTableRemovable::linkHelper(uint hash) {
//     uint next_open = getNextOpen();
//     advanceNextOpen();

//     helper[hash] = next_open;
//     return next_open;
// }

// bool IdHashTableRemovable::isHelperEmpty(uint hash) { return helper[hash] == 0; }

// // Thread
// uint IdHashTableRemovable::next(uint place) { return data[place].next; }
// uint IdHashTableRemovable::prev(uint place) { return data[place].previous; }
// uint IdHashTableRemovable::top(uint hash)   { return helper[hash]; }

// void IdHashTableRemovable::startThread(uint thread_i, uint key, uint value) {
//     data[thread_i].key      = key;
//     data[thread_i].value    = value;
//     data[thread_i].previous = 0;
//     data[thread_i].next     = 0;
// }

// void IdHashTableRemovable::addToThread(uint bottom, uint key, uint value) {
//     uint next_open = getNextOpen();
//     advanceNextOpen();

//     data[bottom].next = next_open;

//     data[next_open].key      = key;
//     data[next_open].value    = value;
//     data[next_open].previous = bottom;
//     data[next_open].next     = 0;
// }

// void IdHashTableRemovable::removeFromThread(uint hash, uint place) {
//     freeNewOpen(place);

//     if (isTopThread(place)) {
//         joinThreadTop(hash, place);
//     } else {
//         joinThreadNotTop(place);
//     }

//     data[place] = HTNode();
// }

// void IdHashTableRemovable::joinThreadTop(uint hash, uint place) {
//     uint next_place;
//     if (!isBottomThread(place)) {
//         next_place = next(place);
//         helper[hash] = next_place;
//         data[next_place].previous = 0;
//     } else {
//         helper[hash] = 0;
//     }
// }

// void IdHashTableRemovable::joinThreadNotTop(uint place) {
//     uint next_place;
//     uint prev_place = prev(place);
//     if (!isBottomThread(place)) {
//         next_place = next(place);
//         data[prev_place].next = next_place;
//         data[next_place].previous = prev_place;
//     } else {
//         data[prev_place].next = 0;
//     }
// }

// uint IdHashTableRemovable::searchThreadToBottom(uint place, uint key) {
//     while (true) {
//         if (isSameKey(place, key)) {
//             return 0;
//         } else if (isBottomThread(place)) {
//             break;
//         } else {
//             place = next(place);
//         }
//     }
//     return place;
// }

// uint IdHashTableRemovable::searchThreadFor(uint place, uint key) {
//     while (true) {
//         if (isSameKey(place, key)) {
//             return place;
//         } else if (isBottomThread(place)) {
//             break;
//         } else {
//             place = next(place);
//         }
//     }
//     return 0;
// }

// bool IdHashTableRemovable::isTopThread(uint thread_i)    { return prev(thread_i) == 0; }
// bool IdHashTableRemovable::isBottomThread(uint thread_i) { return next(thread_i) == 0; }
// bool IdHashTableRemovable::isSameKey(uint thread_i, uint key_compare) { return data[thread_i].key == key_compare; }

// // Open
// uint IdHashTableRemovable::getNextOpen() { return open_spaces[open_marker]; }
// void IdHashTableRemovable::advanceNextOpen() {
//     open_spaces[open_marker] = 0;
//     open_marker = (open_marker + 1) % capacity;
// }

// void IdHashTableRemovable::freeNewOpen(uint place) {
//     open_spaces[next_empty_open_marker] = place;
//     next_empty_open_marker = (next_empty_open_marker + 1) % capacity;
// }

// // === Debugging ===
// void IdHashTableRemovable::printHelper() {
//     std::cout << "Helper: " << std::endl;
//     for (int i=0; i<capacity; i++) {
//         std::cout << helper[i] << std::endl;
//     } 
// }

// void IdHashTableRemovable::printData() {
//     std::cout << "Data: " << std::endl;

//     for (int i=0; i<capacity+1; i++) {
//         data[i].print(i);
//     }
// }

// void IdHashTableRemovable::printOpen() {
//     std::cout << "Open Spaces: " << std::endl;
//     for (int i=0; i<capacity; i++) {
//         if (i == open_marker) {
//             std::cout << "+";
//         } else {
//             std::cout << " ";
//         }
//         if (i == next_empty_open_marker) {
//             std::cout << "-";
//         } else {
//             std::cout << " ";
//         }
//         std::cout << open_spaces[i] << std::endl;
//     }
// }

// void IdHashTableRemovable::print() {
//     printHelper();
//     std::cout << std::endl;
//     printData();
//     std::cout << std::endl;
//     printOpen();
//     std::cout << std::endl;
// }