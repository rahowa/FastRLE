//
// Created by Валентин Евсеенко on 5/13/20.
//

#include "encode_utils.h"


auto encodeRle(cv::Mat&& image) -> std::string {
    std::vector<uchar> pixels(image.total() + 2, 0);
    std::vector<int > result; //((pixels.size() + 1) / 2 + 1);
    std::ostringstream ss;
    std::memmove(pixels.data() + 1, image.data, image.total());

    std::vector<int> runs;
    for(size_t idx = 0; idx < pixels.size() - 1; ++idx){
        if(pixels.at(idx + 1) != pixels.at(idx))
            runs.emplace_back(idx + 1);
    }

    for(size_t idx = 0; idx < runs.size(); idx+=2){
        runs.at(idx + 1) -= runs.at(idx);
    }

    std::move(runs.begin(), runs.end(), std::ostream_iterator<int>(ss, " "));
    return ss.str();
}


auto encodeRle(std::vector<cv::Mat>&& images) -> std::vector<std::string > {
    std::vector<std::string > result(images.size());
    std::transform(std::make_move_iterator(images.begin()),
                   std::make_move_iterator(images.end()),
                   result.begin(),
                   [](auto&& img){return encodeRle(std::move(img));});
    return result;
}


auto encodeRleMt(std::vector<cv::Mat>&& images) -> std::vector<std::string> {
    std::vector<std::string> result(images.size());
    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, images.begin(), images.end());
    std::vector<std::future<std::vector<std::string>>> futures;
    while (!parts.done()){
        auto part = parts.get();
        futures.emplace_back(
                std::async(std::launch::async, [&part](){
                    return encodeRle(part.first, part.second);
                })
            );
    }
    std::for_each(futures.begin(), futures.end(),
            [&](auto&& fut){
        result = mergeVectors(std::move(result), fut.get());
    });
    return result;
}
