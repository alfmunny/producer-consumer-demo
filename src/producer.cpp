#include <producer.h>
#include <util.h>

#include <iostream>

using namespace std::chrono;
using namespace std::chrono_literals;

namespace procon {

Producer::Producer(std::shared_ptr<QueueIntf<DataFrame>> data_frame_queue,
                   std::chrono::milliseconds interval)
    : m_output(m_buffer.m_frame),
      m_queue(data_frame_queue),
      m_interval_ms(interval)
{
}

void Producer::get_data(std::span<char, frame_size> const& output)
{
    std::cout << getCurrentTime() << " Producing " << output.size()
              << " bytes data" << std::endl;
}

void Producer::run()
{
    m_stop = false;

    while (!m_stop) {
        auto t1 = steady_clock::now();
        get_data(m_output);
        m_queue->push(std::move(DataFrame(m_output)));
        auto t2 = steady_clock::now();

        std::unique_lock<std::mutex> lk(m_mtx);
        m_cv.wait_for(lk, m_interval_ms - (t2 - t1),
                      [&]() { return m_stop == true; });
    }
}

void Producer::stop()
{
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        m_stop = true;
    }
    m_cv.notify_one();
}

}  // namespace procon
