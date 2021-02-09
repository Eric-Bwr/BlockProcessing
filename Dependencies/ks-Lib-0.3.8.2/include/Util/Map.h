#pragma once
#include "Vector.h"
#include "Pair.h"

template<typename KEY, typename DATA>
class Map {
private:
    Vector<Pair<KEY, DATA>> map;
    static Pair<KEY, DATA> null;
public:
    Map() = default;
    void append(KEY key, DATA data) {
        map.append(Pair<KEY, DATA>(key, data));
    }

    bool tryAppend(KEY key, DATA data)  {
        if(!has(key)) {
            map.append(Pair<KEY, DATA>(key, data));
            return true;
        }
        return false;
    }

    Pair<KEY, DATA>& find(KEY key)  {
        for(uint64_t i = 0; i < map.length(); i++)
            if(key == map.at(i).getFirst())
                return map.at(i);
        return null;
    }

    DATA get(KEY key) {
        if(has(key))
            return find(key).getSecond();
        return DATA();
    }

    bool has(KEY key) {
        for(uint64_t i = 0; i < map.length(); i++) {
            if(key == map.at(i).getFirst()) {
                return true;
            }
        }
        return false;
    }

    void remove(KEY key) {
        for(uint64_t i = 0; i < map.length(); i++)
            if(key == map.at(i).getFirst())
                return map.erase(i);
    }

    void clear() { map.clear(); }
    bool empty() const { return map.length() == 0; }

    uint64_t size() const { return map.size(); }
    uint64_t length() const { return map.length(); }

    Iterator<Pair<KEY, DATA>> begin() { return Iterator<Pair<KEY, DATA>>(map.data(), map.length()); }
    Iterator<Pair<KEY, DATA>> end() { return Iterator<Pair<KEY, DATA>>(map.data(), map.length()); }

    Pair<KEY, DATA>& operator[] (uint64_t location) { return map[location]; }
};

template<typename KEY, typename DATA>
Pair<KEY, DATA> Map<KEY, DATA>::null;