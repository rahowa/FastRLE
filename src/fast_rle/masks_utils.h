
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


template <typename Iterator>
auto loadMasks(Iterator begin, Iterator end) -> std::vector<cv::Mat> ;

auto loadMasks(std::string&& folder) -> std::vector<cv::Mat> ;

auto loadMasksMt(std::string&& folder) -> std::vector<cv::Mat> ;

auto saveMasks(std::string&& name, cv::Mat&& mask) -> void;

auto saveMasksMt(std::string&& name, cv::Mat&& mask) -> void;


template <typename Iterator>
auto loadMasks(Iterator begin, Iterator end) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> result(std::distance(begin, end));
    std::transform(begin, end, result.begin(),
                    [](const auto & it){return cv::imread(it);});
    return result;
}


