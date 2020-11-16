#ifndef LOAD_MASKS_WRAPPER_H
#define LOAD_MASKS_WRAPPER_H


#include "fast_rle/masks_utils.h"
#include "converters.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <filesystem>


auto loadMasksWrapper(const boost::python::str & pathToMasks) -> boost::python::list ;

#endif // LAOD_MASKS_WRAPPER_H