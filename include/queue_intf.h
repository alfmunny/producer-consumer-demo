#pragma once

namespace procon {
template <typename T>
class QueueIntf {
public:
    virtual ~QueueIntf() = default;
    virtual void push(T const& v) = 0;
    virtual bool pop(T& v) = 0;
    virtual bool empty() = 0;
};
}  // namespace procon
