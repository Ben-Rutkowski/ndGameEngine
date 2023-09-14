#include "uint_hash_table.hpp"
#include <iostream>

UIntHashTable::UIntHashTable(int size_in)
    :capacity{ size_in },
    next_open{ 0 },
    helper_table(size_in, HelperNode()),
    table(size_in, Node()),
    collides{ 0 } { }

UIntHashTable::UIntHashTable()
    :capacity{ 0 },
    next_open{ 0 },
    helper_table(0, HelperNode()),
    table(0, Node()),
    collides{ 0 } { }

unsigned int UIntHashTable::hash(unsigned int key) {
    key = key * 37;
    // key = key << 1;
    return key % capacity;
}

int UIntHashTable::size() { return next_open; }
unsigned int UIntHashTable::operator[](int i) {
    return table[i].key;
}

void UIntHashTable::resize(int n) {
    helper_table.resize(n);
    table.resize(n);

    std::fill_n(helper_table.begin(), n, HelperNode());
    std::fill_n(table.begin(), n, Node());
    capacity  = n;
    next_open = 0;
}

void UIntHashTable::clear() {
    std::fill_n(helper_table.begin(), capacity, HelperNode());
    std::fill_n(table.begin(), capacity, Node());
    next_open = 0;
}

bool UIntHashTable::isElement(unsigned int key) {
    unsigned int hash_id = hash(key);
    if ( helper_table[hash_id].is_empty ) {
        return false;
    } else {
        return searchThread(helper_table[hash_id].node_id, key);
    }
}

unsigned int UIntHashTable::getValue(unsigned int key) {
    unsigned int node_id = helper_table[hash(key)].node_id;

    bool is_end = false;
    while (!is_end) {
        is_end = table[node_id].is_end;

        if ( table[node_id].key == key ) {
            return table[node_id].value;
        } else {
            node_id = table[node_id].next_node;
        }
    }
    return 0;
}

void UIntHashTable::add(unsigned int key, unsigned int value) {
    unsigned int node_id;
    unsigned int hash_id = hash(key);

    if (helper_table[hash_id].is_empty) {
        node_id = activateHelper(hash_id);
        startThread(node_id, key, value);
    } else {
        node_id = helper_table[hash_id].node_id;
        addToThread(node_id, key, value);
    }
}

unsigned int UIntHashTable::activateHelper(unsigned int hash_id) {
    helper_table[hash_id].is_empty = false;
    helper_table[hash_id].node_id = next_open;
    next_open = next_open + 1;

    return helper_table[hash_id].node_id;
}

void UIntHashTable::startThread(unsigned int node_id, unsigned int key, unsigned int value) {
    table[node_id].key   = key;
    table[node_id].value = value;
}

void UIntHashTable::addToThread(unsigned int node_id, unsigned int key, unsigned int value) {
    bool is_end = false;

    while (!is_end) {
        is_end = table[node_id].is_end;

        if (table[node_id].key == key) {
            return;
        } else if (is_end) {
            linkToNewNode(node_id, key, value);
            collides = collides + 1;
        } else {
            node_id = table[node_id].next_node;
        }
    }
}

void UIntHashTable::linkToNewNode(unsigned int node_id, unsigned int key, unsigned int value) {
    table[node_id].is_end = false;
    table[node_id].next_node = next_open;

    table[next_open].key    = key;
    table[next_open].value  = value;
    table[next_open].is_end = true;

    next_open = next_open + 1;
}

bool UIntHashTable::searchThread(unsigned int node_id, unsigned int key) {
    bool is_end = false;
    
    while (!is_end) {
        is_end = table[node_id].is_end;
        if ( table[node_id].key == key ) {
            return true;
        } else {
            node_id = table[node_id].next_node;
        }
    }

    return false;
}

// === Debugging ===
void UIntHashTable::print() {
    std::cout << "Collides: " << collides << std::endl;

    for (int i=0; i<next_open; i++) {
        std::cout << "Key: " << table[i].key;
        std::cout << "  Value: " << table[i].value << std::endl;
    }
}