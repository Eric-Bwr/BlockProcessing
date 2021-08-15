#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<class T>
class SafeQueue {
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
public:
    SafeQueue() : q(), m(), c() {}
    int size(){
        std::lock_guard<std::mutex> lock(m);
        return q.size();
    }
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }
    bool dequeue(T &t, int timeout) {
        std::unique_lock<std::mutex> lock(m);
        bool result = c.wait_for(lock, std::chrono::milliseconds(timeout), [&]() {
            return !q.empty();
        });
        if(result){
            t = q.front();
            q.pop();
            return true;
        }
        return false;
    }
    bool try_dequeue(T &t){
        std::lock_guard<std::mutex> lock(m);
        if(!q.empty()){
            t = q.front();
            q.pop();
            return true;
        }
        return false;
    }
    ~SafeQueue() = default;
};