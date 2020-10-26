#include "load_masks_wrapper.hpp"


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