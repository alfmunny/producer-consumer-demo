#pragma once

#include <chrono>
#include <condition_variable>
#include <memory>
#include <span>

#include "blocking_queue.h"
#include "data_frame.h"

namespace procon {

// Producer class
class Producer {
public:
    Producer(std::shared_ptr<QueueIntf<DataFrame>> data_frame_queue,
             std::chrono::milliseconds interval);
    ~Producer() = default;

public:
    void get_data(std::span<char, frame_size> const& output);
    void run();
    void stop();

private:
    DataFrame m_buffer;
    std::span<char, frame_size> m_output;
    std::shared_ptr<QueueIntf<DataFrame>> m_queue;
    std::condition_variable m_cv;
    std::mutex m_mtx;
    std::atomic_bool m_stop;
    std::chrono::milliseconds m_interval_ms;
};
}  // namespace procon
