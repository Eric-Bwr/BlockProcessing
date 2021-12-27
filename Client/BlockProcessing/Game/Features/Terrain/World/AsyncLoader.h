//Author: Briac
#pragma once

#include <chrono>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <thread>
#include <memory>
#include "BlockProcessing/Framework/Engine/Logger/Logger.h"

template<typename T>
class ThreadSafeQueue {
private:
    struct comp {
        bool operator()(const std::pair<int, T> &a, const std::pair<int, T> &b) {
            return a.first < b.first;
        }
    };
public:
    ThreadSafeQueue() = default;
    virtual ~ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue(ThreadSafeQueue &&) = delete;

    ThreadSafeQueue &operator=(const ThreadSafeQueue &) = delete;
    ThreadSafeQueue &operator=(ThreadSafeQueue &&) = delete;

    size_t push(T &&task, int priority = 0);
    size_t get(T &task, const std::chrono::milliseconds &timeout, bool &success);
    size_t size() const;

    void clear(){
        std::unique_lock<std::mutex> lock(queue_mutex);
        std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, comp> queueEmpty;
        queue.swap(queueEmpty);
    }
private:
    mutable std::mutex queue_mutex;
    std::condition_variable queue_condition;
    std::priority_queue<std::pair<int, T>, std::vector<std::pair<int, T>>, comp> queue;
};

template<typename T>
class AsyncLoader {
public:
    explicit AsyncLoader(int threads = std::thread::hardware_concurrency());
    virtual ~AsyncLoader();

    AsyncLoader(const AsyncLoader &) = delete;
    AsyncLoader(AsyncLoader &&) = delete;

    AsyncLoader &operator=(const AsyncLoader &) = delete;
    AsyncLoader &operator=(AsyncLoader &&) = delete;

    size_t scheduleTask(std::function<T()> &&task, int priority = 0);
    size_t getResult(T &result, const std::chrono::milliseconds &timeout, bool &success);
    std::chrono::milliseconds runAll(std::vector<std::function<T()>> &tasks, std::vector<T> &results);
    std::chrono::milliseconds runAll(std::vector<std::function<T()>> &tasks, const std::function<void(T &&)> &onLoad);
    void exec(std::function<void()> &&f, int priority = 0);

    int getItemsCount() const {
        return items_stored;
    }
    int getItemsProcessed() const {
        return results.size();
    }

    void clear(){
        items_stored = 0;
        tasks.clear();
        results.clear();
    }
private:
    size_t scheduleTask(std::function<T()> &&task, ThreadSafeQueue<T> *results, int priority);
    void thread_loop(int ID);
    std::atomic_bool should_exit = false;
    std::atomic_int items_stored = 0;
    std::vector<std::thread> threads;
    ThreadSafeQueue<std::function<void()>> tasks;
    ThreadSafeQueue<T> results;
};

template<typename T>
AsyncLoader<T>::AsyncLoader(int threads) {
    for (int ID = 0; ID < threads; ID++) {
        std::function<void()> f = [this, ID]() {
            this->thread_loop(ID);
        };
        this->threads.emplace_back(f);
    }
}

template<typename T>
AsyncLoader<T>::~AsyncLoader() {
    LOG<INFO_LVL>("Deleting Async loader....");
    should_exit = true;
    for (std::thread &thread: threads)
        thread.join();
    LOG<INFO_LVL>("Async loader deleted.");
}

template<typename T>
size_t AsyncLoader<T>::scheduleTask(std::function<T()> &&task, int priority) {
    items_stored++;
    return scheduleTask(std::move(task), &results, priority);
}

template<typename T>
size_t AsyncLoader<T>::scheduleTask(std::function<T()> &&task, ThreadSafeQueue<T> *results, int priority) {
    return tasks.push([task = std::move(task), results, priority]() {
        if (results) {
            results->push(std::move(task()), priority);
        } else {
            task();
        }
    });
}

template<typename T>
size_t AsyncLoader<T>::getResult(T &result, const std::chrono::milliseconds &timeout, bool &success) {
    if (items_stored == 0) {
        success = false;
        return 0;
    }
    size_t remaining = results.get(result, timeout, success);
    if (success)
        items_stored--;
    return remaining;
}

template<typename T>
inline std::chrono::milliseconds AsyncLoader<T>::runAll(
        std::vector<std::function<T()> > &tasks,
        std::vector<T> &results) {
    assert(results.empty());
    ThreadSafeQueue<T> l;
    for (auto &t: tasks) {
        scheduleTask(std::move(t), &l);
    }

    auto t0 = std::chrono::system_clock::now();
    while (results.size() != tasks.size()) {
        T res;
        bool success;
        l.get(res, std::chrono::milliseconds(1000), success);
        if (success) {
            results.emplace_back(std::move(res));
        }
    }
    auto t1 = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
}

template<typename T>
inline std::chrono::milliseconds AsyncLoader<T>::runAll(std::vector<std::function<T()> > &tasks, const std::function<void(T &&)> &onLoad) {
    ThreadSafeQueue<T> l;
    for (auto &t: tasks) {
        scheduleTask(std::move(t), &l);
    }

    int completed = 0;
    auto t0 = std::chrono::system_clock::now();
    while (completed < (int) tasks.size()) {
        T res;
        bool success;
        l.get(res, std::chrono::milliseconds(1000), success);
        if (success) {
            onLoad(std::move(res));
            completed++;
        }
    }
    auto t1 = std::chrono::system_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
}

template<typename T>
inline void AsyncLoader<T>::exec(std::function<void()> &&f, int priority) {
    tasks.push(std::move(f), priority);
}

template<typename T>
void AsyncLoader<T>::thread_loop(int ID) {
    std::chrono::milliseconds timeout(1000);
    while (!should_exit) {
        std::function<void()> task;
        bool success = false;
        tasks.get(task, timeout, success);

        if (should_exit) {
            break;
        }

        if (success) {
            try {
                task();
            } catch (std::string &s) {
                LOG<ERROR_LVL, PA>("An exception occurred in thread: ");
                LOG<ERROR_LVL, LA>(ID);
                LOG<ERROR_LVL>(s);
            }
        }
    }
}

template<typename T>
size_t ThreadSafeQueue<T>::push(T &&task, int priority) {
    size_t s;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue.emplace(std::move(std::pair<int, T>(priority, task)));
        s = queue.size();
    }
    queue_condition.notify_one();
    return s;
}

template<typename T>
size_t ThreadSafeQueue<T>::get(T &task, const std::chrono::milliseconds &timeout, bool &success) {
    std::unique_lock<std::mutex> lock(queue_mutex);

    success = queue_condition.wait_for(lock, timeout, [this]() {
        return !queue.empty();
    });

    if (success) {
        task = std::move(queue.top().second);
        queue.pop();
    }

    return queue.size();
}

template<typename T>
size_t ThreadSafeQueue<T>::size() const {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return queue.size();
}
