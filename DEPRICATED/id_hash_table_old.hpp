#ifndef ID_HASH_TABLE_OLD_HPP
#define ID_HASH_TABLE_OLD_HPP

#include <vector>

struct HelperNode {
    bool is_empty = true;
    unsigned int node_id;
};

struct Node {
    unsigned int key;
    unsigned int value;
    unsigned int next_node;
    bool is_end = true;
};


class IdHashTableOld {
private:
    int capacity;
    unsigned int next_open;

    std::vector<HelperNode> helper_table;
    std::vector<Node> table;

    int collides;

public:
    IdHashTableOld();
    IdHashTableOld(int size_in);
    void add(unsigned int key, unsigned int value);

    int size();
    void resizeClear(int n);
    void clear();
    unsigned int operator[](int i);

    bool isElement(unsigned int key);
    unsigned int getValue(unsigned int key);

private:
    unsigned int hash(unsigned int key);

    // If helper is empty, sets it to point to a new node
    // If helper is full, returns the node it points at.
    unsigned int activateHelper(unsigned int hash_id);
    void startThread(unsigned int node_id, unsigned int key, unsigned int value);
    void addToThread(unsigned int node_id, unsigned int key, unsigned int value);
    void linkToNewNode(unsigned int node_id, unsigned int key, unsigned int value);
    bool searchThread(unsigned int node_id, unsigned int key);

// --- Debugging ---
public:
    void print();
};

#endif