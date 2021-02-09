#pragma once
#include <cstdint>
#include <vector>
#include <ostream>
#include "Vector.h"

class String {
private:
    uint64_t strLen = 0;
    Vector<char> _data;

private:
    void popNull();
public:
    String();
    String(const char* string);
    String(const String& string);
    ~String();

    void append(const char* string);
    void append(void* strData);
    void append(char strData);
    void append(const String string);
    void remove(uint64_t begin, uint64_t end);

    char* data();
    Vector<char>& vecData();

    uint64_t size() const;
    uint64_t length() const;
    uint64_t sizeWithTermination() const;
    uint64_t lengthWithTermination() const;
    uint64_t findFirst(String searchFor) const;
    std::vector<uint64_t> find(String searchFor) const;
    std::vector<uint64_t> find(std::vector<String> searchFor) const;
    uint64_t findLast(String searchFor) const;

    char at(uint64_t location) const;
    const char* toString() const;
    String substring(uint64_t begin, uint64_t end);

    bool contains(const char* searchFor) const;
    bool empty() const;

    void popBack();
    void popBack(uint64_t size);
    void popFront();
    void toLower();
    void toUpper();
    void reverse();
    void clear();

    Iterator<const char> begin();
    Iterator<const char> end();
public: //operator overloading
    String& operator= (const char* string);
    String& operator= (const String& string);
    String& operator+= (const char* string);
    String& operator+= (const String& string);
    bool operator> (const String& string);
    bool operator< (const String& string);
    bool operator>= (const String& string);
    bool operator<= (const String& string);
    bool operator== (const String& string);
    bool operator!= (const String& string);
    char& operator[] (uint64_t location);
    friend std::ostream& operator<<(std::ostream& os, const String& string);
};