#include "reading_wrapper.hpp"


auto readCSVWrapper(const boost::python::str & filename) -> boost::python::list {
    auto tmpRes = readCSV(extractString(filename));
    std::vector<RleFileWrapper> result(tmpRes.size());
    std::transform(std::make_move_iterator(tmpRes.begin()),
                   std::make_move_iterator(tmpRes.end()),
                   result.begin(),
                   [](auto && rleFile){return RleFileWrapper(rleFile);});
    return stdToPythonList(std::move(result));
}