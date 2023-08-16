#ifndef ENUM_MAP_HPP
#define ENUM_MAP_HPP

template<typename KeyType, typename EntryType, int Len>
class EnumMap {
private:
    EntryType array[Len];
    
public:
    EnumMap() {}

    void addEntry(KeyType key, EntryType entry) {
        int index = (int)key;
        array[index] = entry;
    }

    EntryType get(KeyType key) {
        return array[(int)key];
    }
};

#endif