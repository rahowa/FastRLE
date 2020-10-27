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
#include <queue>


auto decodeRleMt(RleFiles&& rles) -> std::vector<cv::Mat> ;

auto decodeRle(RleFiles&& rles) ->  std::vector<cv::Mat> ;

auto decodeRle(const RleFile& rleFile) -> cv::Mat ;

template <typename RandAccIterator, typename ResultIterator>
auto decodeRle(RandAccIterator begin, RandAccIterator end, ResultIterator res) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> result(std::distance(begin, end));
    std::transform(std::make_move_iterator(begin),
                   std::make_move_iterator(end),
                   res,
                   [](auto && rle){return decodeRle(std::move(rle));});
    return result;
}

#endif //FAST_RLE_DECODE_UTILS_H
