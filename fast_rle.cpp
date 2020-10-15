//
// Created by Валентин Евсеенко on 6/7/20.
//

#include "python_stuff/about.h"
#include "python_stuff/converters.h"
#include "python_stuff/python_utils.h"
#include "python_stuff/rle_file_wrapper.h"

#include "fast_rle/rle_file.h"
#include "fast_rle/csv_utils.h"
#include "fast_rle/shared_utils.h"
#include "fast_rle/decode_utils.h"
#include "fast_rle/encode_utils.h"
#include "fast_rle/masks_utils.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/stl_iterator.hpp>

#include <numeric>
#include <algorithm>


auto readCSVWrapper(const boost::python::str & filename) -> boost::python::list {
    auto tmpRes = readCSV(extractString(filename));
    std::vector<RleFileWrapper> result(tmpRes.size());
    std::transform(std::make_move_iterator(tmpRes.begin()),
                   std::make_move_iterator(tmpRes.end()),
                   result.begin(),
                   [](auto && rleFile){return RleFileWrapper(rleFile);});
    return stdToPythonList(std::move(result));
}


auto decodeRleWrapper(const boost::python::list & rlesWrapped) -> boost::python::list {    
    auto extractOriginalFn = [](RleFileWrapper&& wrapper){return wrapper.originalFile;};
    auto result = pythonListToStd<RleFileWrapper, RleFile>(rlesWrapped, extractOriginalFn);
    std::vector<cv::Mat> decodedRle(result.size());
    if(result.size() > 1000)
        decodedRle = decodeRleMt(std::move(result));
    else decodedRle = decodeRle(std::move(result));

    return stdToPythonList(cvMatToNumpy(std::move(decodedRle)));
}


auto encodeRleWrapper(const boost::python::list& imagesList) -> boost::python::list {
    auto converterFn = [&](boost::python::numpy::ndarray&& img){return ndarrayToCv(std::move(img));};
    auto images = pythonListToStd<boost::python::numpy::ndarray, cv::Mat>(imagesList, converterFn);
    std::vector<cv::Size> imgSizes(images.size());
    std::transform(images.cbegin(), images.cend(), imgSizes.begin(),
                   [](const auto & img){return cv::Size{img.size[0], img.size[1]};});
    std::vector<RleFileWrapper> result(images.size());
    std::vector<std::string > encodedRles(images.size());

    if(images.size() < 1000)
        encodedRles = encodeRle(std::move(images));
    else encodedRles = encodeRleMt(std::move(images));

    for(size_t idx = 0; idx < result.size(); ++idx){
        result.at(idx) = {boost::python::str(std::string("image_" + std::to_string(idx) + ".png")),
                          boost::python::str(encodedRles.at(idx).c_str()),
                          boost::python::make_tuple(imgSizes.at(idx).width, imgSizes.at(idx).height)};
    }
    return stdToPythonList(std::move(result));
}


auto loadMasksWrapper(const boost::python::str & pathToMasks) -> boost::python::list {
    auto folder = extractString(pathToMasks);
    auto folderIterator = std::filesystem::directory_iterator(std::filesystem::path(folder));
    auto numMasksk = std::distance(std::filesystem::begin(folderIterator), std::filesystem::end(folderIterator));

    std::vector<cv::Mat> masks(numMasksk);
    if(numMasksk < 1000)
        masks = loadMasks(std::move(folder));
    else masks = loadMasksMt(std::move(folder));
    
    return stdToPythonList(cvMatToNumpy(std::move(masks)));
}


auto saveRlesWrapper(const boost::python::str & pathToCsv, const boost::python::list & rlesWrapped) -> void {
    auto extractOriginalFn = [](RleFileWrapper&& wrapper){return wrapper.originalFile;};
    auto result = pythonListToStd<RleFileWrapper, RleFile>(rlesWrapped, extractOriginalFn);
    
    if(result.size() > 10'000)
        saveCSVWithTmpFiles(extractString(pathToCsv), std::move(result));
    else saveCSV(extractString(pathToCsv), std::move(result));
}


BOOST_PYTHON_MODULE(fast_rle_lib)
{
    using namespace boost::python;
    boost::python::numpy::initialize();
    
    class_<RleFileWrapper>("RleFile", init<boost::python::str, boost::python::str, boost::python::tuple>())
            .def("size", &RleFileWrapper::imageSize)
            .def("filename", &RleFileWrapper::filename)
            .def("rle", &RleFileWrapper::rle)
            ;
    
    def("_about", &about);
    def("_read_csv", &readCSVWrapper, args("filename"));
    def("_decode_rle", &decodeRleWrapper, args("rles"));
    def("_encode_rle", &encodeRleWrapper, args("images"));
    def("_save_rle", &saveRlesWrapper, args("filename", "rles"));
}