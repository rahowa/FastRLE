//
// Created by Валентин Евсеенко on 5/11/20.
//

#include "decode_utils.h"



auto decodeRle(RleFiles&& rles) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> result(rles.size());
    std::transform(std::make_move_iterator(rles.begin()),
                   std::make_move_iterator(rles.end()),
                   result.begin(),
            [](auto && rle){return rle2mask(std::move(rle));});
    return result;
}


auto decodeRleMt(RleFiles&& rles) -> std::vector<cv::Mat> {
    std::vector<cv::Mat> masks;

    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, rles.begin(), rles.end());
    
    std::vector<std::future<std::vector<cv::Mat>>> futures;
    while (!parts.done()){
        auto part = parts.get();
        futures.push_back(std::async(std::launch::async,
                           [=]{return rle2mask(part.first, part.second);}));
    }

    std::for_each(std::make_move_iterator(futures.begin()),
                  std::make_move_iterator(futures.end()), 
                  [&](std::future<std::vector<cv::Mat>> && fut){
                      auto tmpVector = fut.get();
                      masks.insert(masks.end(), 
                                   std::make_move_iterator(tmpVector.begin()),
                                   std::make_move_iterator(tmpVector.end()));
                  });
    return masks;
}


auto rle2mask(const RleFile& rleFile) -> cv::Mat {
    auto rle = parseRle(std::move(rleFile.rle));

    std::vector<unsigned char > mask(rleFile.imageSize.width * rleFile.imageSize.height, 0);

    size_t idx = 0;
    auto maskBegin = rle.begin();
    auto maskEnd = rle.end();
    while (maskBegin != maskEnd){
        size_t start = std::max(0, std::stoi(rle.at(idx)) - 1);
        size_t lenght = std::max(0, std::stoi(rle.at(idx + 1)));
        size_t finish = std::max(0ul, start + lenght);
        for(size_t row = start; row < finish; ++row)
            mask.at(row) = 255;

        std::advance(maskBegin, 2);
        idx += 2;
    }
    return {rleFile.imageSize, CV_8UC1, mask.data()};
}