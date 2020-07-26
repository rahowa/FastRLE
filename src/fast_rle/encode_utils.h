//
// Created by Валентин Евсеенко on 5/13/20.
//

#ifndef FAST_RLE_ENCODE_UTILS_H
#define FAST_RLE_ENCODE_UTILS_H


#include "to_parts.h"
#include "shared_utils.h"

#include <opencv2/core.hpp>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <thread>
#include <future>


auto encodeRle(cv::Mat&& image) -> std::string;
auto encodeRle(std::vector<cv::Mat>&& images) -> std::vector<std::string >;

template <typename Iterator>
auto encodeRle(Iterator begin, Iterator end) -> std::vector<std::string > {
    std::vector<std::string > result(std::distance(begin, end));
    std::transform(begin, end, result.begin(),
            [](auto && image){return encodeRle(std::move(image));});
    return result;
}

auto encodeRleMt(std::vector<cv::Mat>&& images) -> std::vector<std::string >;

#endif //FAST_RLE_ENCODE_UTILS_H
