//
// Created by Валентин Евсеенко on 5/9/20.
//

#ifndef FAST_RLE_RLE_FILE_H
#define FAST_RLE_RLE_FILE_H

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct RleFile{
    RleFile() = default;
    RleFile(const RleFile& other) noexcept;
    RleFile(RleFile && other) noexcept;
    RleFile(std::string filename, std::string rle, cv::Size imageSize) noexcept;
    void operator = (const RleFile & other) noexcept ;
    void operator = (RleFile && other) noexcept ;

    std::string filename;
    std::string rle;
    cv::Size imageSize;

    size_t size() const noexcept;
};

using RleFiles = std::vector<RleFile>;


#endif //FAST_RLE_RLE_FILE_H
