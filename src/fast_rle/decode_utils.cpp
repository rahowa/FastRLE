//
// Created by Валентин Евсеенко on 5/11/20.
//

#include "decode_utils.h"
#include <execution>


auto decodeRle(RleFiles&& rles) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> result(rles.size());
    std::transform(std::make_move_iterator(rles.begin()),
                   std::make_move_iterator(rles.end()),
                   result.begin(),
                   [](auto && rle){return decodeRle(std::move(rle));});
    return result;
}


auto decodeRleMt(RleFiles&& rles) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> masks(rles.size());
    std::transform(std::execution::par_unseq, 
                   rles.begin(),
                   rles.end(),
                   masks.begin(),
                   [](auto && rle){return decodeRle(std::move(rle));});
    return masks;
}


auto decodeRle(const RleFile& rleFile) -> cv::Mat {
    auto rle = parseRle(std::move(rleFile.rle));
    cv::Mat mask(1, rleFile.imageSize.height * rleFile.imageSize.width, CV_8UC1);

    auto maskBegin = rle.begin();
    auto maskEnd = rle.end();
    for(size_t idx = 0; maskBegin != maskEnd; std::advance(maskBegin, 2), idx +=2){
        size_t start = std::max(0, std::stoi(rle.at(idx)) - 1);
        size_t lenght = std::max(0, std::stoi(rle.at(idx + 1)));
        size_t finish = std::max(0ul, start + lenght);

        auto it1 = mask.begin<uchar>();
        auto it2 = mask.begin<uchar>();
        std::advance(it1, start);
        std::advance(it2, finish);
        std::fill(it1, it2, 255);
    }
    return mask.reshape(1, rleFile.imageSize.height);;
}
