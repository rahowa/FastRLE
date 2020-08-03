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

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/stl_iterator.hpp>

#include <numeric>
#include <algorithm>


auto readCSVWrapper(const boost::python::str & filename) -> boost::python::list {  //std::vector<RleFileWrapper>
    auto tmpRes = readCSV(extractString(filename));
    auto result = std::accumulate(std::make_move_iterator(tmpRes.begin()),
            std::make_move_iterator(tmpRes.end()),
            std::vector<RleFileWrapper>{},
            [](auto res, auto&& rleFile){
        res.emplace_back(RleFileWrapper(rleFile));
        return std::move(res);
    });
    return stdToPythonList(std::move(result));
}


auto decodeRleWrapper(const boost::python::list & rlesWrapped) -> boost::python::list {    
    auto extractOriginalFn = [](RleFileWrapper&& wrapper){return wrapper.originalFile;};
    auto result = pythonListToStd<RleFileWrapper, RleFile>(rlesWrapped, extractOriginalFn);
    std::vector<cv::Mat> decodedRle(result.size());
    if(result.size() > 100)
        decodedRle = decodeRleMt(std::move(result));
    else 
        decodedRle = decodeRle(std::move(result));

    return stdToPythonList(cvMatToNumpy(std::move(decodedRle)));
}


auto encodeRleWrapper(const boost::python::list& imagesList) -> boost::python::list {
    auto converterFn = [&](boost::python::numpy::ndarray&& img){return ndarrayToCv(std::move(img));};
    auto images = pythonListToStd<boost::python::numpy::ndarray, cv::Mat>(imagesList, converterFn);

    auto imgSizes = std::accumulate(images.cbegin(), images.cend(),
            std::vector<cv::MatSize>{},
             [](auto res, auto & img){
        res.emplace_back(img.size);
        return std::move(res);
    });

    auto imagesCopy = images;
    std::vector<RleFileWrapper> result(imagesCopy.size());
    std::vector<std::string > encodedRles(imagesCopy.size());

    if(imagesCopy.size() < 100)
        encodedRles = encodeRle(std::move(imagesCopy));
    else encodedRles = encodeRleMt(std::move(imagesCopy));

    for(size_t idx = 0; idx < result.size(); ++idx){
        result.at(idx) = {boost::python::str(std::string("image_" + std::to_string(idx) + ".png")),
                                boost::python::str(encodedRles.at(idx).c_str()),
                            boost::python::make_tuple(imgSizes.at(idx)[0], imgSizes.at(idx)[1])};
    }
    return stdToPythonList(std::move(result));
}


BOOST_PYTHON_MODULE(fast_rle)
{
    using namespace boost::python;
    boost::python::numpy::initialize();

    def("about", &about);

    class_<RleFileWrapper>("RleFile", init<boost::python::str, boost::python::str, boost::python::tuple>())
            .def("size", &RleFileWrapper::imageSize)
            .def("filename", &RleFileWrapper::filename)
            .def("rle", &RleFileWrapper::rle)
            ;

    def("read_csv", &readCSVWrapper, args("filename"));
    def("decode_rle", &decodeRleWrapper, args("rles"));
    def("encode_rle", &encodeRleWrapper, args("images"));
}