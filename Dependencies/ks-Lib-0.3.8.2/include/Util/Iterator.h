#pragma once
#include <cstdint>

template<typename DATA>
class Iterator {
private:
    DATA* ptr;
    uint64_t length;
public:
    Iterator(DATA* data) : ptr(data) {}
    Iterator(DATA* data, uint64_t length) : ptr(data), length(length) {}

    Iterator begin() { return Iterator(ptr, length); }
    Iterator end() { return Iterator(ptr + length); }

    bool operator!= (const Iterator& itr) { return ptr != itr.ptr + length; }
    Iterator operator++ () { ++ptr; return *this; }
    const DATA& operator* () const { return *ptr; }
};
