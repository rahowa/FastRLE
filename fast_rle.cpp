//
// Created by Валентин Евсеенко on 6/7/20.
//

#include "python_stuff/about.h"
#include "python_stuff/converters.h"
#include "python_stuff/python_utils.h"
#include "python_stuff/rle_file_wrapper.h"
#include "python_stuff/decoding_wrapper.hpp"
#include "python_stuff/encoding_wrapper.hpp"
#include "python_stuff/saving_wrapper.hpp"
#include "python_stuff/reading_wrapper.hpp"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>


BOOST_PYTHON_MODULE(fast_rle_lib){
    using namespace boost::python;
    boost::python::numpy::initialize();
    
    class_<RleFileWrapper>("RleFile", init<boost::python::str, boost::python::str, boost::python::tuple>())
            .def("size", &RleFileWrapper::imageSize)
            .def("filename", &RleFileWrapper::filename)
            .def("rle", &RleFileWrapper::rle)
            .def_pickle(RleFileWrapeprPickle())
            ;

    class_<DecoderUtils>("_Decoding")
            .def("decode_rle", &DecoderUtils::decode, args("rle_file"))
            .def("decode_rle_list", &DecoderUtils::decodeList, args("rle_files"))
            .def_pickle(DecoderUtilsPickle());
            ;

    class_<EncoderUtils>("_Enciding")
            .def("encode_rle", &EncoderUtils::encode, args("image"))
            .def("encode_rle_list", &EncoderUtils::encodeList, args("images"))
            .def_pickle(EncoderUtilsPickle())
            ;
    
    def("_about", &about);
    def("_read_csv", &readCSVWrapper, args("filename"));
    def("_save_rle", &saveRlesWrapper, args("filename", "rle_file"));
    def("_save_rle_list", &saveRlesListWrapper, args("filename", "rle_files"));
}