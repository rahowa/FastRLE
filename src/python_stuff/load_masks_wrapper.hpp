#include "fast_rle/masks_utils.h"
#include "converters.h"
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <filesystem>


auto loadMasksWrapper(const boost::python::str & pathToMasks) -> boost::python::list ;