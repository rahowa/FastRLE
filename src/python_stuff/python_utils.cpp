//
// Created by Валентин Евсеенко on 7/23/20.
//

#include "python_utils.h"


auto extractString(const boost::python::str & str) -> std::string {
    return boost::python::extract<std::string>(str);
}
