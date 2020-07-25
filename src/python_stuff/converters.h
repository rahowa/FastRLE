//
// Created by Валентин Евсеенко on 7/23/20.
//

#ifndef FAST_RLE_CONVERTERS_H
#define FAST_RLE_CONVERTERS_H


#include <opencv2/core.hpp>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/stl_iterator.hpp>

#include <vector>
#include <iostream>


auto cvMatToNumpy(const cv::Mat& image) -> boost::python::numpy::ndarray ;

auto cvMatToNumpy(cv::Mat&& image) -> boost::python::numpy::ndarray ;

auto cvMatToNumpy(const std::vector<cv::Mat>& images) -> std::vector<boost::python::numpy::ndarray> ;

auto cvMatToNumpy(std::vector<cv::Mat>&& images) -> std::vector<boost::python::numpy::ndarray> ;

auto ndarrayToCv(const boost::python::numpy::ndarray& ndarr) -> cv::Mat ;

auto ndarrayToCv(boost::python::numpy::ndarray&& ndarr) -> cv::Mat ;

template<typename Sequence>
auto stdToPythonList(const Sequence & seq) -> boost::python::list {
    boost::python::list result;
    std::for_each(seq.cbegin(), seq.cend(),
                  [&](const auto & it){result.append(it);});
    return result;
}

template<typename T>
auto pythonListToStd(const boost::python::list& list) -> std::vector<T> {
    return {boost::python::stl_input_iterator<T>(list),
            boost::python::stl_input_iterator<T>()};
}

template<typename InputT, typename  OutputT, typename Function>
auto pythonListToStd(const boost::python::list& list, Function fnConverter) -> std::vector<OutputT> {
    std::vector<InputT> tmp {boost::python::stl_input_iterator<InputT>(list),
                             boost::python::stl_input_iterator<InputT>()};
    std::vector<OutputT> result(tmp.size());
    std::transform(std::make_move_iterator(tmp.begin()),
                   std::make_move_iterator(tmp.end()),
                   result.begin(),
                   fnConverter);
    return result;
}


#endif //FAST_RLE_CONVERTERS_H
