#include "converters.h"
#include "rle_file_wrapper.h"
#include "fast_rle/csv_utils.h"
#include "python_utils.h"


#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <vector>


auto readCSVWrapper(const boost::python::str & filename) -> boost::python::list ;