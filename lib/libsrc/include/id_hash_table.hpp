#ifndef ID_HASH_TABLE_HPP
#define ID_HASH_TABLE_HPP

typedef unsigned int uint;

#include <vector>
#include <iostream>

struct HTNode {
    uint key;
    uint value;
    uint previous = 0;
    uint next     = 0;

    void print(int i) {
        std::cout << "[" << i << "] Key: " << key;
        std::cout << " Value: " << value << std::endl;
        std::cout << "    Previous: " << previous;
        std::cout << "    Next: " << next << std::endl << std::endl;
    }
};

class IdHashTableBase {
protected:
    uint capacity;

    std::vector<uint> helper;      // Hash index, points to data
    std::vector<HTNode> data;      // Contains the threads, ie linked lists
    std::vector<uint> open_spaces; // Contains the indices for the next open spaces

    uint open_marker;

public:
    IdHashTableBase();
    IdHashTableBase(uint n);

    void add(uint key, uint value);
    void add(uint key);

    uint value(uint key);
    bool isElement(uint key);

    int  size();
    void clear();
    void resizeClear(int n);


protected:
    uint hashFunction(uint key);
    bool isTableFull();

    // Helper
    uint linkHelper(uint hash);
    bool isHelperEmpty(uint hash);

    // Thread
    uint next(uint place);
    uint prev(uint place);
    uint top(uint hash);

    void startThread(uint thread_i, uint key, uint value);
    void addToThread(uint bottom, uint key, uint value);
    uint searchThreadToBottom(uint place, uint key); // 0 if set has key, else bottom of thread
    uint searchThreadForKey(uint place, uint key); // place of key if set has key, else 0

    bool isTopThread(uint thread_i);
    bool isBottomThread(uint thread_i);
    bool isSameKey(uint thread_i, uint key_compare);

    // Open Spaces
    uint getNextOpen();
    void stepNextOpen();

// --- Debugging ---
public:
    void print();
    void printHelper();
    void printData();
    void printOpen();
};

class IdHashTableStatic : public IdHashTableBase {
public:
    IdHashTableStatic();
    IdHashTableStatic(uint n);

    int begin();
    int end();
    uint operator[](int i);
};

class IdHashTableDynamic : public IdHashTableBase {
private:
    uint next_empty_open_marker;

public:
    IdHashTableDynamic();
    IdHashTableDynamic(uint n);

    bool remove(uint key);
    bool canForceKey();
    uint forceGetKey(int i);

private:
    void removeFromThread(uint hash, uint place);
    void joinThreadTop(uint hash, uint place);
    void joinThreadNotTop(uint place);
    void freeNewOpen(uint place);
};

// class IdHashTableRemovable {
// private:
//     uint capacity;

//     std::vector<uint> helper;      // Hash index, points to data
//     std::vector<HTNode> data;      // Contains the threads, ie linked lists
//     std::vector<uint> open_spaces; // Contains the indices for the next open spaces

//     uint open_marker;
//     uint next_empty_open_marker;

// public:
//     IdHashTableRemovable();
//     IdHashTableRemovable(uint n);

//     void add(uint key, uint value);
//     void add(uint key);
//     bool remove(uint key);

//     uint operator[](int i);
//     uint value(uint key);
//     bool isElement(uint key);

//     int  size();
//     void clear();
//     void resizeClear(int n);


// private:
//     uint hashFunction(uint key);
//     bool isTableFull();
//     bool isTableEmpty();

//     // Helper
//     uint linkHelper(uint hash);
//     bool isHelperEmpty(uint hash);

//     // Thread
//     uint next(uint place);
//     uint prev(uint place);
//     uint top(uint hash);

//     void startThread(uint thread_i, uint key, uint value);
//     void addToThread(uint bottom, uint key, uint value);
//     void removeFromThread(uint hash, uint place);
//     void joinThreadTop(uint hash, uint place);
//     void joinThreadNotTop(uint place);

//     /* Searches a thread for a key, if it is found returns 0
//        else returns the bottom of the thread. */ 
//     uint searchThreadToBottom(uint place, uint key); 
//     /* Searches thread for key, if found returns the place,
//        else returns 0.
//     */
//     uint searchThreadFor(uint place, uint key);

//     bool isTopThread(uint thread_i);
//     bool isBottomThread(uint thread_i);
//     bool isSameKey(uint thread_i, uint key_compare);

//     // Open
//     uint getNextOpen();
//     void advanceNextOpen();

//     void freeNewOpen(uint place);

// // --- Debugging ---
// public:
//     void print();
//     void printHelper();
//     void printData();
//     void printOpen();
// };

#endif