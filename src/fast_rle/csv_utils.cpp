//
// Created by Валентин Евсеенко on 5/11/20.
//

#include "csv_utils.h"

auto readCSV(std::string&& filename) -> RleFiles {
    io::CSVReader<3> in(filename);
    in.read_header(io::ignore_extra_column, "image_name", "rle", "image_shape");

    RleFiles resStruct;
    std::string tmpFilename;
    std::string tmpRle;
    std::string tmpImgSize;
    while (in.read_row(tmpFilename, tmpRle, tmpImgSize)){
        resStruct.emplace_back(RleFile{std::move(tmpFilename),
                                       std::move(tmpRle),
                                       std::move(strToSize(std::move(tmpImgSize)))});
    }
    return resStruct;
}


auto saveCSV(std::string &&filename, RleFiles &&encodings) -> void {
    std::fstream resultCsv(filename);
    resultCsv << "image_name," << "rle," << "image_shape,";
    std::for_each(encodings.begin(), encodings.end(),
            [&resultCsv](auto && it){
                resultCsv << std::move(it.filename) << ","
                          << std::move(it.rle) << ","
                          << std::move(it.imageSize) << '\n';
    });
    resultCsv.close();
}


auto saveCSVMt(std::string &&filename, RleFiles &&encodings) -> void {
    std::mutex m;
    std::fstream resultCsv(filename);
    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, encodings.begin(), encodings.end());
    std::vector<std::future<void>> futures(encodings.size());

    auto saveInstancesFn = [&](auto begin, auto end){
        std::for_each(begin, end, [&](auto&& it){
            std::lock_guard lock(m);
            resultCsv << std::move(it.filename) << ","
                      << std::move(it.rle) << ","
                      << std::move(it.imageSize) << '\n';
        });
    };

    while (!parts.done()){
        auto part = parts.get();
        futures.emplace_back(std::async(saveInstancesFn, part.first, part.second));
    }
    std::for_each(futures.begin(), futures.end(), [](auto&& it){it.get();});
    resultCsv.close();
}


