#include "masks_utils.h"


auto loadMasks(std::string&& folder) -> std::vector<cv::Mat> {
    auto masksLocations = filesInside(std::move(folder));
    std::vector<cv::Mat> result(masksLocations.size());
    std::transform(masksLocations.begin(), masksLocations.end(),
                   std::back_inserter(result),
                   [](auto&& path){return cv::imread(path);});
    return result;
}


auto loadMasksMt(std::string&& folder) -> std::vector<cv::Mat>{
    auto masksLocations = filesInside(std::move(folder));
    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, masksLocations.begin(), masksLocations.end());
    std::vector<std::future<std::vector<cv::Mat>>> futures;

    while (!parts.done()){
        auto part = parts.get();
        futures.emplace_back(std::async(
                [&]{return loadMasks(part.first, part.second);}
            )
        );
    }
    
    std::vector<cv::Mat> result;
    std::for_each(futures.begin(), futures.end(),
            [&](auto&& future){result = mergeVectors(std::move(result), future.get());});
    return result;
}