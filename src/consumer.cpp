#include <consumer.h>
#include <util.h>

#include <iostream>

namespace procon {

Consumer::Consumer(std::shared_ptr<QueueIntf<DataFrame>> data_frame_queue,
                   std::function<void(DataFrame&)> filter)
    : m_queue(data_frame_queue), m_filter(std::move(filter))
{
}

void Consumer::send_data(std::span<const char, frame_size> const& output)
{
    std::cout << getCurrentTime() << " Sending " << output.size()
              << " bytes data" << std::endl;
}

void Consumer::run()
{
    m_stop = false;

    DataFrame buffer;
    while (!m_stop && m_queue->pop(buffer)) {
        m_filter(buffer);
        send_data(std::span<char, frame_size>(buffer.m_frame));
    }
}

void Consumer::stop()
{
    while (!m_queue->empty()) {
    }

    m_stop = true;
    DataFrame buffer;
    m_queue->push(buffer);
}

}  // namespace procon
