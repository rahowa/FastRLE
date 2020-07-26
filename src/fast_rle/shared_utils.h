//
// Created by Валентин Евсеенко on 5/9/20.
//

#ifndef FAST_RLE_SHARED_UTILS_H
#define FAST_RLE_SHARED_UTILS_H

#include <vector>
#include <opencv2/core.hpp>
#include <boost/algorithm/string.hpp>
#include <ostream>
#include <filesystem>


template<typename T>
auto mergeVectors(std::vector<T>&& vTo, std::vector<T>&& vFrom) -> std::vector<T> {
    vTo.insert(vTo.end(),
               std::make_move_iterator(vFrom.begin()),
               std::make_move_iterator(vFrom.end()));
    return vTo;
}

auto strToSize(std::string&& imgSizeStr) ->  cv::Size;

auto sizeToStr(cv::Size&& imgSize) -> std::string;

auto parseRle(std::string maskRle) -> std::vector<std::string>;

auto filesInside(std::string && folder) -> std::vector<std::string> ;

#endif //FAST_RLE_SHARED_UTILS_H
