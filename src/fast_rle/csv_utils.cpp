//
// Created by Валентин Евсеенко on 5/11/20.
//

#include "csv_utils.h"


auto concatenateFiles(const std::vector<std::string> & filenames) -> void {
    std::ofstream baseFile(filenames.at(0), std::ios::app | std::ios::binary);
    std::for_each_n(filenames.crbegin(), filenames.size() - 1, 
    [&](const std::string & filename){baseFile << std::ifstream(filename, std::ios::binary).rdbuf();});
}


auto removeFiles(std::vector<std::string> && filenames) -> void {
    auto removeFn = [](std::string && filename){std::remove(filename.c_str());};
    std::for_each(std::make_move_iterator(filenames.begin()),
                  std::make_move_iterator(filenames.end()),
                  removeFn);
}


auto readCSV(std::string&& filename) -> RleFiles {
    io::CSVReader<3> in(filename);
    in.read_header(io::ignore_extra_column, "image_name", "rle", "image_shape");

    RleFiles resStruct;
    std::string tmpFilename;
    std::string tmpRle;
    std::string tmpImgSize;
    while (in.read_row(tmpFilename, tmpRle, tmpImgSize)){
        resStruct.emplace_back(
                std::move(tmpFilename),
                std::move(tmpRle),
                std::move(strToSize(std::move(tmpImgSize))
            )
        );
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
        }
    );
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


auto saveCSVWithTmpFiles(std::string && filename, RleFiles&& encodings) -> void {
    auto numParts = std::max(2u, std::thread::hardware_concurrency());
    auto parts = ToParts(numParts, encodings.begin(), encodings.end());
    std::vector<std::future<void>> futures;

    auto saveInstancesFn = [&](auto begin, auto end, CSVWriter& csvWriter){
        std::for_each(begin, end, [&](auto && it){
             csvWriter.write(std::move(it.filename),
                             std::move(it.rle),
                             std::move(it.imageSize));
        });
    };

    std::vector<std::string> tempFilenames;
    tempFilenames.emplace_back(filename);
    for(size_t idx = 1; idx < numParts; ++idx)
        tempFilenames.emplace_back("tmp_" + std::to_string(idx) + ".csv");

    std::vector<CSVWriter> csvFiles;
    csvFiles.emplace_back(tempFilenames.at(0), "image_name", "rle", "image_shape");
    for(size_t idx = 1; idx < numParts; ++idx)
        csvFiles.emplace_back(tempFilenames.at(idx));

    size_t tmpFileCounter{0};
    while (!parts.done()){
        auto part = parts.get();
        futures.emplace_back(std::async(saveInstancesFn, part.first, part.second, std::ref(csvFiles.at(tmpFileCounter))));
        ++tmpFileCounter;
    }

    std::for_each(futures.begin(), futures.end(), [](auto && it){it.get();});
    concatenateFiles(tempFilenames);
    removeFiles({tempFilenames.begin() + 1, tempFilenames.end()});
}