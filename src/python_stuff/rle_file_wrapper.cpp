//
// Created by Валентин Евсеенко on 7/23/20.
//

#include "rle_file_wrapper.h"



RleFileWrapper::RleFileWrapper(const RleFile & rleFile):
    filename_(rleFile.filename),
    rle_(rleFile.rle),
    imageSize_(boost::python::make_tuple(rleFile.imageSize.height, rleFile.imageSize.width)),
    originalFile(rleFile)
{
};

RleFileWrapper::RleFileWrapper(boost::python::str filename, boost::python::str rle, boost::python::tuple imageSize):
    filename_(std::move(filename)),
    rle_(std::move(rle)),
    imageSize_(std::move(imageSize)) {
    originalFile = {extractString(filename_),extractString(rle_),
                    cv::Size{boost::python::extract<int>(imageSize_[0]),boost::python::extract<int>(imageSize_[1])}};
}

RleFileWrapper::RleFileWrapper(const std::string & filename, const std::string & rle, const std::tuple<int, int> & imageSize):
    filename_(filename),
    rle_(rle),
    imageSize_(imageSize) {
    originalFile = {extractString(filename_),extractString(filename_),
                    cv::Size{boost::python::extract<int>(imageSize_[0]),boost::python::extract<int>(imageSize_[1])}};
}


auto RleFileWrapper::imageSize() const noexcept -> boost::python::tuple {
    return boost::python::make_tuple(originalFile.imageSize.height, originalFile.imageSize.width);
}

auto RleFileWrapper::rle() const noexcept -> boost::python::str {
    return boost::python::str(originalFile.rle.c_str());
}

auto RleFileWrapper::filename() const noexcept -> boost::python::str {
    return boost::python::str(originalFile.filename.c_str());
}
