//
// Created by Валентин Евсеенко on 5/9/20.
//

#include "shared_utils.h"


auto strToSize(std::string&& imgSizeStr) ->  cv::Size {
    std::vector<std::string> imageShapes;
    boost::split(imageShapes, std::move(imgSizeStr), boost::is_any_of("x"));
    return {std::move(std::stoi(imageShapes.at(0))),
            std::move(std::stoi(imageShapes.at(1)))};
}

auto sizeToStr(cv::Size&& imgSize) -> std::string {
    std::ostringstream ss;
    ss << imgSize.width << "x" << imgSize.height;
    return ss.str();
}

auto parseRle(std::string maskRle) -> std::vector<std::string> {
    std::vector<std::string> parsedRLE;
    boost::split(parsedRLE, std::move(maskRle), boost::is_any_of(" "));
    return parsedRLE;
}

auto filesInside(std::string && folder) -> std::vector<std::string> {
    auto filesIt = std::filesystem::directory_iterator(folder);
    std::vector<std::string> allFiles;
    std::transform(std::make_move_iterator(std::filesystem::begin(filesIt)),
                   std::make_move_iterator(std::filesystem::end(filesIt)),
                   std::back_inserter(allFiles),
                   [](auto && file){return file.path();});
    return allFiles;
}
