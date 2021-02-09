#pragma once
#include <cstdint>
#include "Memory.h"

template<typename BASE>
class InitializerList {
private:
    BASE* arr;
    uint64_t aSize = 0, iaSize = 0;

    template<typename FIRST>
    void processList(FIRST f) {
        if(typeid(f) == typeid(*arr)) {
            arr[iaSize] = f;
            iaSize++;
        }
    }

    template<typename FIRST, typename ... BLIST>
    void processList(FIRST f, BLIST ... list) {
        processList(f);
        processList((list)...);
    }
public:
    template<typename ... BLIST>
    InitializerList(BLIST &&... list) {
        aSize = sizeof...(list);
        arr = (BASE *)calloc(aSize, sizeof(BASE));
        processList((list)...);
        if (iaSize != aSize)
            arr = (BASE *) recalloc(arr, aSize, iaSize, sizeof(BASE));
    }

    InitializerList() : arr(0) {}

    ~InitializerList() {
        free(arr);
    }

    BASE& getListRef() const { return arr[0]; }
    BASE* getListPtr() const { return arr; }
    uint64_t getListSize() const { return iaSize; }

    Iterator<BASE> begin() { return Iterator<BASE>(arr, aSize); }
    Iterator<BASE> end() { return Iterator<BASE>(arr, aSize); }
};