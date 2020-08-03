#include "log_latency.h"
#include "shared_utils.h"
#include "csv_utils.h"
#include "decode_utils.h"
#include "encode_utils.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <numeric>
#include <vector>
#include <utility>
#include <filesystem>
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>


int main(int argc, char* argv[]) {
    auto rles = readCSV("assets/test_masks.csv");
    {
        LATENCY_OF("mt decode");
        auto res = decodeRleMt(std::move(rles));
        // auto _ = encodeRleMt(std::move(res));
    }
    
    {
        LATENCY_OF("one thread decode");
        auto res = decodeRle(std::move(rles));
        // auto _ = encodeRle(std::move(res));
    }
    return 0;
}
