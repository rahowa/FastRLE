//
// Created by Валентин Евсеенко on 5/9/20.
//

#ifndef FAST_RLE_LOG_LATENCY_H
#define FAST_RLE_LOG_LATENCY_H

#include <string>
#include <chrono>
#include <iostream>

class LogLatency {
public:
    LogLatency(std::string &&logName);
    ~LogLatency();

private:
    std::chrono::steady_clock::time_point start;
    std::string logName;
};
#define LATENCY_OF(object) LogLatency duration(#object)

#endif //FAST_RLE_LOG_LATENCY_H
