
#include "to_parts.h"
#include "shared_utils.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include <filesystem>
#include <future>
#include <thread>


auto loadMasks(std::string&& folder) -> std::vector<cv::Mat> ;

auto loadMasksMt(std::string&& folder) -> std::vector<cv::Mat> ;

auto saveMasks(std::string&& name, cv::Mat&& mask) -> void;

auto saveMasksMt(std::string&& name, cv::Mat&& mask) -> void;

template <typename Iterator>
auto loadMasks(Iterator begin, Iterator end) -> std::vector<cv::Mat> {
    return std::accumulate(begin, end, std::vector<cv::Mat>{},
            [](auto res, auto it){
        res.emplace_back(cv::imread(it));
        return std::move(res);
    });
}
