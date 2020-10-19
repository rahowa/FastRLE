//
// Created by Валентин Евсеенко on 5/13/20.
//

#include "encode_utils.h"


auto encodeRle(cv::Mat&& image) -> std::string {
    std::vector<uchar> pixels(image.total() + 2, 0);
    std::vector<int > result; //((pixels.size() + 1) / 2 + 1);
    std::ostringstream ss;
    std::memcpy(pixels.data() + 1, image.data, image.total());

    std::vector<int> runs;
    size_t runsCounter = 0;
    runs.reserve(pixels.size());
    for(size_t idx = 0; idx < pixels.size() - 1; ++idx){
        if(pixels.at(idx + 1) != pixels.at(idx))
            runs.at(runsCounter) = idx + 1;
            ++runsCounter;
    }
    runs.shrink_to_fit();
    
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
    std::vector<std::string> encodings; //(images.size());
    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, images.begin(), images.end());
    std::vector<std::future<std::vector<std::string>>> futures;
    while (!parts.done()){
        auto part = parts.get();
        futures.push_back(std::async(std::launch::async,
                            [=]{return encodeRle(part.first, part.second);})
            );
    }

    std::for_each(std::make_move_iterator(futures.begin()),
                  std::make_move_iterator(futures.end()), 
                  [&](std::future<std::vector<std::string>> && fut){
                      auto tmpVector = fut.get();
                      encodings.insert(encodings.end(), 
                                   std::make_move_iterator(tmpVector.begin()),
                                   std::make_move_iterator(tmpVector.end()));
                  });
    return encodings;
}
