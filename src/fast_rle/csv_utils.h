//
// Created by Валентин Евсеенко on 5/11/20.
//

#ifndef FAST_RLE_CSV_UTILS_H
#define FAST_RLE_CSV_UTILS_H

#include "rle_file.h"
#include "shared_utils.h"
#include "fast-cpp-csv-parse/csv.h"
#include "to_parts.h"

#include <fstream>
#include <thread>
#include <future>

auto readCSV(std::string&& filename) -> RleFiles;

auto saveCSV(std::string&& filename, RleFiles&& encodings) -> void;

auto saveCSVMt(std::string&& filename, RleFiles&& encodings) -> void;

#endif //FAST_RLE_CSV_UTILS_H
