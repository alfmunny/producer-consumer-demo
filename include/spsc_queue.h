#pragma once
#include <boost/lockfree/spsc_queue.hpp>
#include <condition_variable>
#include <queue>

#include "queue_intf.h"

namespace procon {

// Ringbuffer implementation for queue. Single Producer/Single Consumer
template <typename T>
class SPSCQueue : public QueueIntf<T> {
public:
    SPSCQueue(size_t size) : m_queue(size) {}
    ~SPSCQueue() = default;

    void push(const T& v) override
    {
        m_queue.push(v);
        m_cv.notify_one();
    }

    bool empty() override { return m_queue.empty(); }

    bool pop(T& v) override
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        while (m_queue.empty()) {
            m_cv.wait(lk);
        }
        v = m_queue.front();
        m_queue.pop();
        return true;
    }

private:
    boost::lockfree::spsc_queue<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

}  // namespace procon
