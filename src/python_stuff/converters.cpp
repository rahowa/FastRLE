//
// Created by Валентин Евсеенко on 7/23/20.
//

#include "converters.h"


auto cvMatToNumpy(const cv::Mat& image) -> boost::python::numpy::ndarray {
    boost::python::tuple shape = boost::python::make_tuple(image.rows, image.cols, 1);
    boost::python::tuple stride = boost::python::make_tuple(image.cols * sizeof(uchar), sizeof(uchar), sizeof(uchar));
    boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<uchar>();
    return boost::python::numpy::from_data(image.clone().data, dt, shape, stride, boost::python::object());
}


auto cvMatToNumpy(cv::Mat&& image) -> boost::python::numpy::ndarray {
    boost::python::tuple shape = boost::python::make_tuple(image.rows, image.cols, 1);
    boost::python::tuple stride = boost::python::make_tuple(image.cols * sizeof(uchar), sizeof(uchar), sizeof(uchar));
    boost::python::numpy::dtype dt = boost::python::numpy::dtype::get_builtin<uchar>();
    return boost::python::numpy::from_data(image.clone().data, dt, shape, stride, boost::python::object());
}


auto cvMatToNumpy(const std::vector<cv::Mat>& images) -> std::vector<boost::python::numpy::ndarray> {
    std::vector<boost::python::numpy::ndarray> result;
    std::transform(images.begin(),
                   images.end(),
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
    const Py_intptr_t* shape = ndarr.get_shape();
    char* dtype_str = boost::python::extract<char *>(boost::python::str(ndarr.get_dtype()));

    // variables for creating Mat object
    int rows = shape[0];
    int cols = shape[1];
    int channel = shape[2];
    int depth;

    // you should find proper type for c++. in this case we use 'CV_8UC3' image, so we need to create 'uchar' type Mat.
    if (!strcmp(dtype_str, "uint8")) {
        depth = CV_8U;
    }
    else {
        std::cout << "wrong dtype error" << std::endl;
        return cv::Mat();
    }

    cv::Mat mat = cv::Mat(rows, cols, CV_MAKETYPE(depth, channel));
    memcpy(mat.data, ndarr.get_data(), sizeof(uchar) * rows * cols * channel);
    return mat;
}


auto ndarrayToCv(boost::python::numpy::ndarray&& ndarr) -> cv::Mat {
    const Py_intptr_t* shape = ndarr.get_shape();
    char* dtype_str = boost::python::extract<char *>(boost::python::str(ndarr.get_dtype()));

    int rows = shape[0];
    int cols = shape[1];
    int channel = shape[2];
    int depth;

    if (!strcmp(dtype_str, "uint8")) {
        depth = CV_8U;
    }
    else {
        std::cout << "wrong dtype error" << std::endl;
        return cv::Mat();
    }

    cv::Mat mat = cv::Mat(rows, cols, CV_MAKETYPE(depth, channel));
    memmove(mat.data, ndarr.get_data(), rows * cols * channel);
    return mat;
}