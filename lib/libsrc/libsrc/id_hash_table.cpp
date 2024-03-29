#include "id_hash_table.hpp"
#include <iostream>
#include <numeric>

// ================ IdHashTableBase ================

IdHashTableBase::IdHashTableBase()
    :IdHashTableBase(0) {}

IdHashTableBase::IdHashTableBase(uint n)
    :capacity_size{ n },
    open_marker{ 0 },
    helper(capacity_size),
    data(capacity_size+1),
    open_spaces(capacity_size) {

    // helper.resize(capacity_size);
    std::fill_n(helper.begin(), capacity_size, 0);
    
    // data.resize(capacity_size + 1);

    // open_spaces.resize(capacity_size);
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

// === Hash ===
bool IdHashTableBase::isTableFull() { return open_spaces[open_marker] == 0; }
uint IdHashTableBase::hashFunction(uint key) {
    key = key * 37;
    return key % capacity_size;
}

// === Interface ===
void IdHashTableBase::add(uint key) { add(key, 0, 0, 0); }
void IdHashTableBase::add(uint key, uint value) { add(key, value, 0, 0); }
void IdHashTableBase::add(uint key, uint value_1, uint value_2, uint value_3) {
    // if (isTableFull()) {
    //     std::cout << "Table is Full!" << std::endl;
    //     return;
    // }
    uint place;
    uint hash = hashFunction(key);
    if (isHelperEmpty(hash)) {
        place = linkHelper(hash);
        startThread(place, key, value_1, value_2, value_3);
    } else {
        place = top(hash);
        place = searchThreadToBottom(place, key);
        if (place != 0) {
            addToThread(place, key, value_1, value_2, value_3);
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
        return data[place].value_1 - 1;
    } else {
        return 0;
    }
}

// uint IdHashTableBase::value_2(uint key) {
//     uint place = top(hashFunction(key));
//     place = searchThreadForKey(place, key);
//     if (place != 0 ) {
//         return data[place].value_2 - 1;
//     } else {
//         return 0;
//     }
// }

// uint IdHashTableBase::value_3(uint key) {
//     uint place = top(hashFunction(key));
//     place = searchThreadForKey(place, key);
//     if (place != 0 ) {
//         return data[place].value_3 - 1;
//     } else {
//         return 0;
//     }
// }

bool IdHashTableBase::hasElement(uint key) {
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
    resetMarkers();
    // open_marker = 0;

    helper.resize(capacity_size);
    data.resize(capacity_size + 1);
    open_spaces.resize(capacity_size);

    std::fill_n(helper.begin(), capacity_size, 0);
    std::fill_n(data.begin(), capacity_size + 1, HTNode());
    std::iota(open_spaces.begin(), open_spaces.end(), 1);
}

void IdHashTableBase::forceSetValue(uint value, int i) {
    data[i+1].value_1 = value + 1;
}

uint IdHashTableBase::forceGetValue(int i) {
    return data[i+1].value_1 - 1;
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

void IdHashTableBase::startThread(uint thread_i, uint key, uint value_1, uint value_2, uint value_3) {
    data[thread_i].key      = key;
    data[thread_i].value_1  = value_1 + 1;
    // data[thread_i].value_2  = value_2 + 1;
    // data[thread_i].value_3  = value_3 + 1;
    data[thread_i].previous = 0;
    data[thread_i].next     = 0;
}

void IdHashTableBase::addToThread(uint bottom, uint key, uint value_1, uint value_2, uint value_3) {
    uint next_open = getNextOpen();
    stepNextOpen();

    data[bottom].next = next_open;

    data[next_open].key      = key;
    data[next_open].value_1  = value_1 + 1;
    // data[next_open].value_2  = value_2 + 1;
    // data[next_open].value_3  = value_3 + 1;
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

void IdHashTableBase::resetMarkers() {
    open_marker = 0;
}

// === Debugging ===
void IdHashTableBase::printHelper() {
    std::cout << "Helper: " << std::endl;
    for (int i=0; i<capacity_size; i++) {
        std::cout << helper[i] << std::endl;
    } 
}

void IdHashTableBase::printDataSmall() {
    std::cout << "Data: " << std::endl;

    for (int i=0; i<capacity_size; i++) {
        data[i+1].printSmall(i+1);
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
    if (open_spaces.size() == 0) {
        return 0;
    } else if (open_marker == 0 && open_spaces[0] == 0) {
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

void IdHashTableDynamic::resetMarkers() {
    open_marker = 0;
    next_empty_open_marker = 0;
}