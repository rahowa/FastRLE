//
// Created by Валентин Евсеенко on 7/23/20.
//

#ifndef FAST_RLE_PYTHON_UTILS_H
#define FAST_RLE_PYTHON_UTILS_H

#include <boost/python.hpp>
#include <string>


auto extractString(const boost::python::str & str) -> std::string;

#endif //FAST_RLE_PYTHON_UTILS_H
