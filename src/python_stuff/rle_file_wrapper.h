//
// Created by Валентин Евсеенко on 7/23/20.
//

#ifndef FAST_RLE_RLE_FILE_WRAPPER_H
#define FAST_RLE_RLE_FILE_WRAPPER_H


#include "python_utils.h"
#include "fast_rle/rle_file.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <opencv2/core.hpp>


struct RleFileWrapper {
    RleFileWrapper() = default;
    explicit RleFileWrapper(const RleFile & rleFile);
    RleFileWrapper(boost::python::str filename, boost::python::str rle, boost::python::tuple imageSize);
    RleFileWrapper(const std::string & filename, const std::string & rle, const std::tuple<int, int> & imageSize);

    RleFile originalFile;
    boost::python::str rle_;
    boost::python::str filename_;
    boost::python::tuple imageSize_;

    [[nodiscard]] auto imageSize() const noexcept -> boost::python::tuple ;
    [[nodiscard]] auto rle() const noexcept -> boost::python::str;
    [[nodiscard]] auto filename() const noexcept -> boost::python::str;
};



struct RleFileWrapeprPickle: boost::python::pickle_suite {
    static auto getinitargs(const RleFileWrapper& rleFile) -> boost::python::tuple;
};

#endif //FAST_RLE_RLE_FILE_WRAPPER_H
