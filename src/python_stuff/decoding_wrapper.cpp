#include "decoding_wrapper.hpp"


auto DecoderUtils::decodeRleWrapper(const RleFileWrapper& rle) -> boost::python::numpy::ndarray {    
    auto origRle = rle.originalFile;
    auto decodedRle = decodeRle(std::move(origRle));
    return cvMatToNumpy(std::move(decodedRle));
}

auto DecoderUtils::decodeRleListWrapper(const boost::python::list & rlesWrapped) -> boost::python::list {    
    auto extractOriginalFn = [](RleFileWrapper&& wrapper){return wrapper.originalFile;};
    auto result = pythonListToStd<RleFileWrapper, RleFile>(rlesWrapped, extractOriginalFn);
    auto decodedRle = decodeRle(std::move(result));
    return stdToPythonList(cvMatToNumpy(std::move(decodedRle)));
}


static DecoderUtilsPickle::getinitargs(const DecoderUtils& decUtils) -> boost::python::tuple {
    return boost::python::make_tuple();
}

