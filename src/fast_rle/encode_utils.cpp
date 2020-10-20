//
// Created by Валентин Евсеенко on 5/13/20.
//

#include "encode_utils.h"


auto encodeRle(cv::Mat&& image) -> std::string {
    image = image.reshape(1, 1);

    size_t count = 0;
    std::string encoding = "";
    for(size_t i = 0; i < image.total() - 1; ++i){
        count = 1;
        if((int)image.at<uchar>(i) == 255){
            while(image.at<uchar>(i) == image.at<uchar>(i + 1)){
                ++count;
                ++i;
            }
            encoding += std::to_string(i - 1) + " " + std::to_string(count) + " ";
        }
    }
    return encoding;
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
    encodings.reserve(images.size());

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
                      encodings.insert(encodings.cend(), 
                                   std::make_move_iterator(tmpVector.begin()),
                                   std::make_move_iterator(tmpVector.end()));
                  });
    return encodings;
}
