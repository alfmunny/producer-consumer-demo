#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <span>

#include "blocking_queue.h"
#include "data_frame.h"

namespace procon {

class Consumer {
public:
    using QueueIntfPtr = std::shared_ptr<QueueIntf<DataFrame>>;

    Consumer(QueueIntfPtr data_frame_queue,
             std::function<void(DataFrame&)> filter);
    ~Consumer() = default;

public:
    void send_data(std::span<const char, frame_size> const& output);
    void run();
    void stop();

private:
    QueueIntfPtr m_queue;
    std::atomic_bool m_stop;
    std::mutex m_mtx;
    std::function<void(DataFrame&)> m_filter;
};

}  // namespace procon
