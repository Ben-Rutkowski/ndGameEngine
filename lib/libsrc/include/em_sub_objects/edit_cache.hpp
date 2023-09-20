#ifndef EDIT_CACHE_HPP
#define EDIT_CACHE_HPP

#include "math/vector.hpp"

/* CLASS TEMPLATE: EditCache

An EditCache contains a stack of objects. An EditCache also 
contains references to every edge and every face that contains
the point.

*/

typedef unsigned int Id;
typedef std::array<Id,2> Id2;
typedef std::array<Id,3> Id3;
typedef std::array<Id,4> Id4;

template<typename T>
class EditCache {
protected:
    std::vector<T> data;
    std::vector<std::vector<Id>> pair_cache_1;
    std::vector<std::vector<Id>> pair_cache_2;

public:
    T&    operator[](int i) { return data[i]; }
    void* dataPtr()  { return (void*)&data[0]; }    
    int   dataSize() { return data.size()*sizeof(T); }
    int   dataLen()  { return data.size(); }

protected:
    Id addData(T data_object) {
        data.push_back(data_object);
        return data.size() - 1;
    }

    Id createData() {
        data.push_back(T());
        return data.size() - 1;
    }

    void pushPairCache1() { pair_cache_1.push_back(std::vector<Id>()); }
    void pushPairCache2() { pair_cache_2.push_back(std::vector<Id>()); }

    void pushPairCache1(int reserve) {
        pair_cache_1.push_back(std::vector<Id>());
        pair_cache_1.back().reserve(reserve); }
    void pushPairCache2(int reserve) {
        pair_cache_2.push_back(std::vector<Id>());
        pair_cache_2.back().reserve(reserve); }

    void pairIndexCache1(Id root_id, Id pair_id) 
        { pair_cache_1[root_id].push_back(pair_id); }
    void pairIndexCache2(Id root_id, Id pair_id)
        { pair_cache_2[root_id].push_back(pair_id); }    

    void emptyIndexCache1(Id root_id)
        { pair_cache_1[root_id].clear(); }
    void emptyIndexCache2(Id root_id)
        { pair_cache_2[root_id].clear(); }

    int pairedLen1(Id data_id) { return pair_cache_1[data_id].size(); }
    int pairedLen2(Id data_id) { return pair_cache_2[data_id].size(); }

    Id getPairedCache1(Id data_id, int i) { return pair_cache_1[data_id][i]; }
    Id getPairedCache2(Id data_id, int i) { return pair_cache_2[data_id][i]; }
};

#endif