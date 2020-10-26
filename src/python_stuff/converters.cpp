//
// Created by Валентин Евсеенко on 7/23/20.
//

#include "converters.h"


auto cvMatToNumpy(const cv::Mat& image) -> boost::python::numpy::ndarray {
    boost::python::tuple shape = boost::python::make_tuple(image.rows, image.cols, 1);
    boost::python::tuple stride = boost::python::make_tuple(image.cols * sizeof(uchar), sizeof(uchar), sizeof(uchar));
    boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<uchar>();
    return boost::python::numpy::from_data(image.data, dt, shape, stride, boost::python::object());
}


auto cvMatToNumpy(cv::Mat&& image) -> boost::python::numpy::ndarray {
    boost::python::tuple shape = boost::python::make_tuple(image.rows, image.cols, 1);
    boost::python::tuple stride = boost::python::make_tuple(image.cols * sizeof(uchar), sizeof(uchar), sizeof(uchar));
    boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<uchar>();
    return boost::python::numpy::from_data(image.data, dt, shape, stride, boost::python::object());
}


auto cvMatToNumpy(const std::vector<cv::Mat>& images) -> std::vector<boost::python::numpy::ndarray> {
    std::vector<boost::python::numpy::ndarray> result;
    std::transform(images.cbegin(),
                   images.cend(),
                   std::back_inserter(result),
                   [](cv::Mat img){return cvMatToNumpy(img);});
    return result;
}


auto cvMatToNumpy(std::vector<cv::Mat>&& images) -> std::vector<boost::python::numpy::ndarray> {
    std::vector<boost::python::numpy::ndarray> result;
    std::transform(std::make_move_iterator(images.begin()),
                   std::make_move_iterator(images.end()),
                   std::back_inserter(result),
                   [](cv::Mat && img){return cvMatToNumpy(std::move(img));});
    return result;
}


auto ndarrayToCv(const boost::python::numpy::ndarray& ndarr) -> cv::Mat {
    const int numDims = ndarr.get_nd();
    const Py_intptr_t* shape = ndarr.get_shape();
    const auto dtypeStr  = extractString(boost::python::str(ndarr.get_dtype()));

    // variables for creating Mat object
    size_t rows = shape[0];
    size_t cols = shape[1];
    size_t channels;

    if (numDims == 3)
        channels = shape[2];
    else channels = 1;

    int depth;
    // you should find proper type for c++. in this case we use 'CV_8UC3' image, so we need to create 'uchar' type Mat.
    if (dtypeStr == "uint8") {
        depth = CV_8U;
    }
    else {
        std::cout << "wrong dtype error" << std::endl;
        return cv::Mat();
    }

    cv::Mat mat = cv::Mat(rows, cols, CV_MAKETYPE(depth, channels));
    memcpy(mat.data, ndarr.get_data(), sizeof(uchar) * rows * cols * channels);
    return mat;
}
