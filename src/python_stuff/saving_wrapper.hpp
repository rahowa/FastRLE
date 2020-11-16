#ifndef SAVING_WRAPPER_H
#define SAVING_WRAPPER_H


#include "converters.h"
#include "rle_file_wrapper.h"
#include "fast_rle/csv_utils.h"

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>


auto saveRlesWrapper(const boost::python::str & pathToCsv, const RleFileWrapper& rleWrapper) -> void ;

auto saveRlesListWrapper(const boost::python::str & pathToCsv, const boost::python::list & rlesWrapped) -> void ;


#endif