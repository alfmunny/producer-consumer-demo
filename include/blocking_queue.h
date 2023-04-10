#pragma once
#include <condition_variable>
#include <queue>

#include "queue_intf.h"

namespace procon {
// Queue, blocking on pop while empty.
template <typename T>
class BlockingQueue : public QueueIntf<T> {
public:
    ~BlockingQueue() = default;

    void push(const T& v) override
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        m_queue.push(v);

        if (m_queue.size() == 1) {
            m_cv.notify_one();
        }
    }

    bool empty() override
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        return m_queue.empty();
    }

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
    std::queue<T> m_queue;
    std::mutex m_mtx;
    std::condition_variable m_cv;
};

}  // namespace procon
