#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <iostream>
#include <sstream>
#include <string>

template<class T>
class SafeQueue {
private:
    const std::string name;
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
public:
    SafeQueue(const std::string& name) : name(name), q(), m(), c() {}

    ~SafeQueue() {}

    int size(){
        std::lock_guard<std::mutex> lock(m);
        return q.size();
    }

    // Add an element to the queue.
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        std::ostringstream ss;
        ss <<"Attempting to dequeue from " <<name <<" until an item is ready or the timeout expires.\n";
       // std::cout <<"Enqueueing into " <<name <<"\n";
        q.push(t);
        c.notify_one();
    }
    // Get the "front"-element.
    // If the queue is empty, wait till a element is avaiable or until the timeout expires.
    bool dequeue(T &t, int timeout) {
        std::ostringstream ss;
        ss <<"Attempting to dequeue from " <<name <<" until an item is ready or the timeout expires.\n";
       // std::cout <<ss.str();

        std::unique_lock<std::mutex> lock(m);
        bool result = c.wait_for(lock, std::chrono::milliseconds(timeout), [&]() {
            return !q.empty();
        });
        if(result){
            ss.str("");
            ss <<"Timeout did not expire, dequeueing from " <<name <<"\n";
            //std::cout <<ss.str();

            t = q.front();
            q.pop();
            return true;
        }
        ss.str("");
        ss <<"Timeout expired when dequeuing from " <<name <<"\n";
       // std::cout <<ss.str();
        return false;
    }


    // Get the "front"-element.
    // Returns false if the queue is empty
    bool try_dequeue(T &t){
        std::lock_guard<std::mutex> lock(m);
        if(!q.empty()){
            std::ostringstream ss;
            ss <<"Successfully dequeued from " <<name <<" in try_dequeue.\n";
            //std::cout <<ss.str();
            t = q.front();
            q.pop();
            return true;
        }
        std::ostringstream ss;
        ss <<"Could not dequeue from " <<name <<" in try_dequeue because the queue is empty.\n";
        //std::cout <<ss.str();
        return false;
    }
};