//
// Created by Валентин Евсеенко on 5/11/20.
//

#ifndef FAST_RLE_DECODE_UTILS_H
#define FAST_RLE_DECODE_UTILS_H

#include "csv_utils.h"
#include "to_parts.h"
#include <algorithm>
#include <vector>
#include <opencv2/core.hpp>
#include <future>
#include <thread>


auto decodeRleMt(RleFiles&& rles) noexcept -> std::vector<cv::Mat> ;

auto decodeRle(RleFiles&& rles) noexcept ->  std::vector<cv::Mat> ;

auto rle2mask(const RleFile& rleFile) -> cv::Mat ;

template <typename RandAccIterator>
auto rle2mask(RandAccIterator begin, RandAccIterator end) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> result(std::distance(begin, end));
    std::transform(std::make_move_iterator(begin),
                   std::make_move_iterator(end),
                   result.begin(),
                   [](auto && rle){return rle2mask(std::move(rle));});
    return result;
}

#endif //FAST_RLE_DECODE_UTILS_H
