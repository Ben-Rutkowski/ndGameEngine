#ifndef SEARCH_TREE_HPP
#define SEARCH_TREE_HPP

#include <vector>

/* CLASS: SearchTree
*/

class SearchTree {
private:
    std::vector<int> tree;

public:
    SearchTree(int size);

    void add(int value);
    void remove(int value);
    void isElement(int value);

private:
    
};

#endif