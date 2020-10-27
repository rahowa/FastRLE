#include "encoding_wrapper.hpp"


auto EncoderUtils::encode(const boost::python::numpy::ndarray& image) -> RleFileWrapper {
    auto cvMatImage = ndarrayToCv(image);
    auto imageSize = cvMatImage.size();
    auto encodedRle = encodeRle(std::move(cvMatImage));
    return {boost::python::str(std::string("image_" + std::to_string(rand() % 1000000) + ".png")),
            boost::python::str(encodedRle.c_str()),
            boost::python::make_tuple(imageSize.height, imageSize.width)};
}

auto EncoderUtils::encodeList(const boost::python::list& imagesList) -> boost::python::list {
    auto converterFn = [](const boost::python::numpy::ndarray& img){return ndarrayToCv(img);};
    auto images = pythonListToStd<boost::python::numpy::ndarray, cv::Mat>(imagesList, converterFn);
    
    std::vector<cv::Size> imgSizes(images.size());
    std::transform(images.cbegin(), images.cend(), imgSizes.begin(),
                   [](const auto & img){return cv::Size{img.size[0], img.size[1]};});
    std::vector<RleFileWrapper> result(images.size());

    auto encodedRles = encodeRle(std::move(images));
    for(size_t idx = 0; idx < result.size(); ++idx){
        result.at(idx) = {boost::python::str(std::string("image_" + std::to_string(idx) + ".png")),
                          boost::python::str(encodedRles.at(idx).c_str()),
                          boost::python::make_tuple(imgSizes.at(idx).height, imgSizes.at(idx).width)};
    }
    return stdToPythonList(std::move(result));
}


auto EncoderUtilsPickle::getinitargs(const EncoderUtils& encUtils) -> boost::python::tuple {
    return boost::python::make_tuple();
}

