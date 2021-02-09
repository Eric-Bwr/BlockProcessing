#pragma once

#include <malloc.h>
#include <cstdint>
#include <iostream>
#include "Memory.h"
#include "Iterator.h"
#include "InitializerList.h"

template<typename BASE>
class Vector {
private:
    uint64_t _size = 0;
    uint64_t allocSize = 0;
    BASE* _data;

private:
    void reserve() {
        _data = (BASE*)recalloc(_data, _size, _size + 1, sizeof(BASE));
    }


public:
    Vector() {
        _data = (BASE*)malloc(0);
    }

    Vector(const Vector<BASE>& vec) {
        _size =  vec.length();
        _data = (BASE*)calloc(_size, sizeof(BASE));
        memcpy(_data, vec._data, _size * sizeof(BASE));
    }

    ~Vector() {
        free(_data);
    }

    void reserve(uint64_t elementCount) {
        _data = (BASE*)recalloc(_data, _size, _size + elementCount, sizeof(BASE));
        allocSize = elementCount;
    }

    void append(BASE newData) {
        if(allocSize == 0)
            reserve();
        else
            allocSize--;
        _size++;
        _data[_size - 1] = newData;
    }

    void append(Vector<BASE>& newData) {
        reserve(newData.length());
        memcpy(_data + _size, newData.data(), newData.length());
    }

    void clear() {
        _size = allocSize =  0;
        free(_data);
        _data = (BASE*)malloc(0);
    }

    void erase(uint64_t location) {
        _size--;
        BASE* d = (BASE*)calloc(_size, sizeof(BASE));
        short del = 0;
        for(uint64_t i = 0; i < _size + 1; i++) {
            if(i != location) {
                d[i - del] = _data[i];
            } else {
                del = 1;
            }
        }
        free(_data);
        _data = (BASE*)recalloc(d, _size, _size, sizeof(BASE));
    }

    void erase(uint64_t beginLocation, uint64_t endlocation) {
        if(endlocation > _size) endlocation = _size;
        if(beginLocation > _size) beginLocation = _size;
        uint64_t locDif = endlocation - (beginLocation - 1);
        _size -= locDif;
        BASE* d = (BASE*)calloc(_size, sizeof(BASE));
        short del = 0;
        for(uint64_t i = 0; i < _size + locDif; i++) {
            if (i < beginLocation || i > endlocation) {
                d[i - del] = _data[i];
            } else {
                del++;
            }
        }
        free(_data);
        _data = (BASE*)recalloc(d, _size, _size, sizeof(BASE));
    }

    BASE& at(uint64_t location) const {  return _data[location]; }
    BASE& first() const { if(_size > 0) return at(0);  else {BASE t; return t;}}
    BASE& last() const { if(_size > 0) return at(_size - 1);  else {BASE t; return t;} }
    BASE* data() const { return _data; }

    uint64_t length() const { return _size; }
    uint64_t size() const { return _size; }

    Iterator<BASE> begin() const { return Iterator<BASE>(_data, _size); }
    Iterator<BASE> end() const { return Iterator<BASE>(_data, _size); }

    bool empty() const { return _size == 0; }

    const Vector<BASE>& operator= (const InitializerList<BASE>& initList) {
        this->clear();
        this->append(initList);
        return *this;
    }

    const Vector<BASE>& operator+= (const InitializerList<BASE>& initList) {
        this->append(initList);
        return *this;
    }

    const Vector<BASE>& operator= (const Vector<BASE>& vec) {
        this->clear();
        this->append(vec);
        return *this;
    }

    const Vector<BASE>& operator+= (const Vector<BASE>& vec) {
        this->append(vec);
        return *this;
    }

    BASE& operator[] (const uint64_t location) { return _data[location]; }
};