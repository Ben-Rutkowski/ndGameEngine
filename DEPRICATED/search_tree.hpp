#ifndef SEARCH_TREE_HPP
#define SEARCH_TREE_HPP

#include <vector>
#include <iostream>

/* CLASS: SearchTree
*/

#define NOCHILD 0b00000000
#define RIGHTCHILD 0b00000001
#define LEFTCHILD 0b00000010
#define BOTHCHILD 0b00000011

template<typename T>
struct stNode {
    T value;
    char children = NOCHILD;
};

template<typename T>
class SearchTree {
private:
    int height;
    int size;
    std::vector<stNode<T>> tree;
    std::vector<T> iterator;

public:
    SearchTree() {}
    SearchTree(int h)
        :height{ h },
        size{ (1<<h)-1 },
        tree(size),
        iterator(height) {}

    void add(int value);
    void remove(int value);
    void isElement(int value);    

private:
    bool hasRight(int i) {
        int right_index = rightIndex(i);

        if (right_index >= size) {
            return false;
        }

        char child = tree[right_index].children;
        return hasRightChild(child);
    }

public:
    static int leftIndex(int i)   { return 2*i + 1; }
    static int rightIndex(int i)  { return 2*i + 2; }
    static int parentIndex(int i) { return floor((i-1)/2); }

    static char addRightChild(char child)    { return child | RIGHTCHILD; }
    static char removeRightChild(char child) { return child & LEFTCHILD; }
    static char addLeftChild(char child)     { return child | LEFTCHILD; }
    static char removeLeftChild(char child)  { return child & RIGHTCHILD; }
    
    static bool hasLeftChild(char child)  { return child & LEFTCHILD; }
    static bool hasRightChild(char child) { return child & RIGHTCHILD; }

// --- Debuging ---
public:
    void addForce(int i, stNode<T> node) {
        tree[i] = node;
    }

    void addForceLeft(int i, T value) {
        int left = leftIndex(i);
        tree[i].children = addLeftChild(tree[i].children);
        tree[left].value = value;
    }

    void addForceRight(int i, T value) {
        int right = rightIndex(i);
        tree[i].children = addRightChild(tree[i].children);
        tree[right].value = value;
    }

    void printNode(int i) {
        std::cout << "Node " << i << ":" << std::endl;
        std::cout << "    Value: " << tree[i].value << std::endl;
        printChild(tree[i].children);
    }

    void printChild(char data) {
        std::cout << "    Children: ";
        
        switch (data) {
        case 0b00000000:
            std::cout << "NONE" << std::endl;
            break;
        case 0b00000010:
            std::cout << "LEFT" << std::endl;
            break;
        case 0b00000001:
            std::cout << "RIGHT" << std::endl;
            break;
        case 0b00000011:
            std::cout << "BOTH" << std::endl;
            break;
        default: break;
        }

        std::cout << std::endl;
    }

    void print() {
        std::cout << "Size: " << size << std::endl;
        std::cout << "Height: " << height << std::endl;
    }
};

#endif