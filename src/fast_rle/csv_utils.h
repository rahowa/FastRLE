//
// Created by Валентин Евсеенко on 5/11/20.
//

#ifndef FAST_RLE_CSV_UTILS_H
#define FAST_RLE_CSV_UTILS_H

#include "rle_file.h"
#include "to_parts.h"
#include "csv_writer.hpp"
#include "shared_utils.h"
#include "fast-cpp-csv-parse/csv.h"

#include <fstream>
#include <thread>
#include <future>


auto concatenateFiles(std::vector<std::string> && filenames) -> void;

auto removeFiles(std::vector<std::string> && filenames) -> void;

auto readCSV(std::string&& filename) -> RleFiles;

auto saveCSV(std::string&& filename, RleFiles&& encodings) -> void;

auto saveCSV(std::string &&filename, RleFile&& encodings) -> void;

auto saveCSVMt(std::string&& filename, RleFiles&& encodings) -> void;

auto saveCSVWithTmpFiles(std::string && filename, RleFiles&& encodings) -> void;


#endif //FAST_RLE_CSV_UTILS_H