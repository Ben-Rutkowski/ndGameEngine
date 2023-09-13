#ifndef EDIT_CACHE_HPP
#define EDIT_CACHE_HPP

#include "math/vector.hpp"

/* CLASS TEMPLATE: EditCache

An EditCache contains a stack of objects. An EditCache also 
contains references to every edge and every face that contains
the point.

*/

template<typename T>
class EditCache {
protected:
    std::vector<T> data;
    std::vector<std::vector<unsigned int>> index_cache_1;
    std::vector<std::vector<unsigned int>> index_cache_2;
    std::vector<std::vector<unsigned int>> index_cache_3;

public:
    T&    operator[](int i) { return data[i]; }
    void* dataPtr()  { return (void*)&data[0]; }    
    int   dataSize() { return data.size()*sizeof(T); }
    int   dataLen()  { return data.size(); }

protected:
    unsigned int addData(T& data_object) {
        data.push_back(data_object);
        return data.size() - 1;
    }

    unsigned int createData() {
        data.push_back(T());
        return data.size() - 1;
    }

    void pushCache1() { index_cache_1.push_back(std::vector<unsigned int>()); }
    void pushCache2() { index_cache_2.push_back(std::vector<unsigned int>()); }
    void pushCache3() { index_cache_3.push_back(std::vector<unsigned int>()); }

    void pairIndexCache1(unsigned int root_id, unsigned int pair_id) 
        { index_cache_1[root_id].push_back(pair_id); }
    void pairIndexCache2(unsigned int root_id, unsigned int pair_id)
        { index_cache_2[root_id].push_back(pair_id); }    
    void pairIndexCache3(unsigned int root_id, unsigned int pair_id)
        { index_cache_3[root_id].push_back(pair_id); }   

    int cacheNum1(int data_id) { return index_cache_1[data_id].size(); }
    int cacheNum2(int data_id) { return index_cache_2[data_id].size(); }
    int cacheNum3(int data_id) { return index_cache_3[data_id].size(); }

    unsigned int getCache1(int data_id, int i) { return index_cache_1[data_id][i]; }
    unsigned int getCache2(int data_id, int i) { return index_cache_2[data_id][i]; }
    unsigned int getCache3(int data_id, int i) { return index_cache_3[data_id][i]; }
};

#endif