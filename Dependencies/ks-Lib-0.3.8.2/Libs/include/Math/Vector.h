#pragma once

#include <cstdarg>
#include <cstdint>
#include <typeinfo>
#include "../TypeConversion/TypeConversion.h"


namespace m {
     template<typename Type, uint64_t size>
     class Vector{
     private:
         uint64_t vecSize = size;
         Type* type;
         uint64_t indexNext = 0;
     public:
         Vector() = default;
         Vector(Type t...) {
             va_list list;
             va_start(list, t);
             type = (float*)malloc(4);
             for(uint64_t i = 0; i < size; i++) {
                 if(ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(float).name()) || ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(double).name()))
                    type[i] = i == 0 ? t : va_arg(list, double);
                 else if(ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(bool).name()) || ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(short).name()) || ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(int).name()))
                     type[i] = i == 0 ? t : va_arg(list, int);
                 else if(ntc::isCStringEqual((char*)typeid(Type).name(), (char*)typeid(long).name()))
                     type[i] = i == 0 ? t : va_arg(list, long);
                 else {
                     va_end(list);
                     return;
                 }
             }
             va_end(list);
         }
         ~Vector() {
             //free(type);
         }

         Type* getData() { return type; }
         Type at(uint64_t index) { if(index < vecSize) return type[index]; else return Type(); }
         Type getNext() { if(indexNext < vecSize) return type[indexNext++]; else { indexNext = 0; return Type(); }}

         void clear() {
            delete[] type;
            type = nullptr;
         }

         int getSize() { return vecSize; }
     };
}
