#pragma once

#include <time.h>

#include <chrono>
#include <ctime>
#include <sstream>

namespace {

std::string getCurrentTime()
{
    std::ostringstream os;
    struct tm tm;
    time_t now = time(0);

    localtime_r(&now, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    os << buf;
    return os.str();
}

}  // namespace
