#pragma once

template<typename DATA0, typename DATA1>
class Pair {
private:
    DATA0 data0;
    DATA1 data1;
    bool valid = false;
public:
    Pair() = default;
    ~Pair() = default;
    Pair(DATA0 d0, DATA1 d1) : data0(d0), data1(d1) { valid = true; }

    void setFirst(DATA0 d0) { data0 = d0;  valid = true; }
    void setSecond(DATA1 d1) { data1 = d1;  valid = true; }

    DATA0 getFirst() const { return data0; }
    DATA1 getSecond() const { return data1; }

    bool isValid() { return valid; }

    bool operator== (const bool& b) { return valid == b; }
    bool operator!= (const bool& b) { return valid != b; }
    bool operator= (const bool& b) { return valid != b; }
    operator bool() const { return valid; }
};