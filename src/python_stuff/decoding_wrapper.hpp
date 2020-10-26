#include "fast_rle/decode_utils.h"
#include "rle_file_wrapper.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>


struct DecoderUtils{
    auto decode(const RleFileWrapper& rle) -> boost::python::numpy::ndarray;
    auto decodeList(const boost::python::list & rlesWrapped) -> boost::python::list;
};


struct DecoderUtilsPickle: boost::python::pickle_suite {
    static auto getinitargs(const DecoderUtils& decUtils) -> boost::python::tuple ;
};
