//
// Created by Валентин Евсеенко on 5/9/20.
//

#include "shared_utils.h"

auto strToSize(std::string&& imgSizeStr) ->  cv::Size {
    std::vector<std::string> imageShapes;
    boost::split(imageShapes, imgSizeStr, boost::is_any_of("x"));
    return {std::stoi(imageShapes.at(0)),
            std::stoi(imageShapes.at(1))};
}

auto sizeToStr(cv::Size&& imgSize) -> std::string {
    std::ostringstream ss;
    ss << imgSize.width << "x" << imgSize.height;
    return ss.str();
}

auto parseRle(std::string maskRle) -> std::vector<std::string> {
    std::vector<std::string> parsedRLE;
    boost::split(parsedRLE, maskRle, boost::is_any_of(" "));
    return parsedRLE;
}

