#pragma once
#include <span>
#include <vector>

namespace procon {
constexpr size_t frame_size = 16 * 1024 * 1024;

struct DataFrame {
    DataFrame() { m_frame.resize(frame_size); }
    DataFrame(std::span<const char, frame_size> const& frame)
    {
        m_frame.resize(frame_size);
        m_frame.assign(frame.begin(), frame.end());
    }

    std::vector<char> m_frame;
};

}  // namespace procon
