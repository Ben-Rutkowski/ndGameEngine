#ifndef ENUM_MAP_HPP
#define ENUM_MAP_HPP

#include <array>

/* CLASS TEMPLATE: EnumMap
    KeyType : type of the keys
    EntryType : type of the entrys
    N : length of the map

An EnumMap is a data structure that associates an entry to an
enum class. EnumMaps have a fixed length and are normally definied
once before the application begins.

======== ATTRIBUTES ========
- array<EntryType,Len> map


======== METHODS ========
-------- CONSTRUCTOR --------
- EnumMap : Does not initialize the array. That is if an entry 
    is retrived that is not defined then will return a null EntryType.

-------- GETS AND SETS --------
- addEntry : Adds an entry to the map.
    ==Parameters==
        - KeyType key : The key for the entry.
        - EntryType entry : The entry for the key.

- get : Returns a copy of the entry at the given key.
     ==Parameters==
        - KeyType key : The key whose entry will be returned.
    ==Return==
        - EntryType : The entry at the given key.

*/

template<typename KeyType, typename EntryType, int N>
class EnumMap {
private:
    std::array<EntryType,N> map;
    
public:
    EnumMap() {}

    void addEntry(KeyType key, EntryType entry) {
        int index = (int)key;
        map[index] = entry;
    }

    EntryType get(KeyType key) {
        return map[(int)key];
    }
};

#endif