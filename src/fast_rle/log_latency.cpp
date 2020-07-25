//
// Created by Валентин Евсеенко on 5/9/20.
//

#include "log_latency.h"

LogLatency::LogLatency(std::string &&logName) :
        logName(logName) {
    start = std::chrono::steady_clock::now();
}

LogLatency::~LogLatency() {
    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[" << logName << "] " << diff.count() << " [ms]" << '\n';
}
