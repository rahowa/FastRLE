#include "saving_wrapper.hpp"


auto saveRlesWrapper(const boost::python::str & pathToCsv, const RleFileWrapper& rleWrapper) -> void {
    auto origRleFile = rleWrapper.originalFile;
    saveCSV(extractString(pathToCsv), std::move(origRleFile));
}


auto saveRlesListWrapper(const boost::python::str & pathToCsv, const boost::python::list & rlesWrapped) -> void {
    auto extractOriginalFn = [](RleFileWrapper&& wrapper){return wrapper.originalFile;};
    auto result = pythonListToStd<RleFileWrapper, RleFile>(rlesWrapped, extractOriginalFn);
    
    if(result.size() > 10'000)
        saveCSVWithTmpFiles(extractString(pathToCsv), std::move(result));
    else saveCSV(extractString(pathToCsv), std::move(result));
}
