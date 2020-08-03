//
// Created by Валентин Евсеенко on 5/9/20.
//

#include "rle_file.h"


RleFile::RleFile(std::string filename, std::string rle, cv::Size imageSize) noexcept:
    filename(std::move(filename)), 
    rle(std::move(rle)),
    imageSize(std::move(imageSize))
{
};

size_t RleFile::size() const noexcept {
    return filename.size();
}


RleFile::RleFile(const RleFile& other) noexcept:
    filename(other.filename),
    rle(other.rle),
    imageSize(other.imageSize)
{
};

RleFile::RleFile(RleFile && other) noexcept:
    filename(std::move(other.filename)),
    rle(std::move(other.rle)),
    imageSize(std::move(other.imageSize))
{
};

void RleFile::operator = (const RleFile & other) noexcept {
    filename = other.filename;
    rle = other.rle;
    imageSize = other.imageSize;
}

void RleFile::operator = (RleFile && other) noexcept  {
    filename = std::move(other.filename);
    rle = std::move(other.rle);
    imageSize = std::move(other.imageSize);
}