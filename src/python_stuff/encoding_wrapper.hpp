#include "fast_rle/encode_utils.h"
#include "rle_file_wrapper.h"
#include "converters.h"
#include "python_utils.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>


struct EncoderUtils{
    auto encode(const boost::python::numpy::ndarray& image) -> RleFileWrapper;
    auto encodeList(const boost::python::list& imagesList) -> boost::python::list ;
};


struct EncoderUtilsPickle: boost::python::pickle_suite {
    static auto getinitargs(const EncoderUtils& encUtils) -> boost::python::tuple ;
};
