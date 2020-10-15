#include <fstream>
#include <mutex>
#include <iostream>


class CSVWriter {
    std::ofstream csvFile;
    std::mutex m;

    public:
    CSVWriter(CSVWriter && other):
        csvFile(std::move(other.csvFile))
    {
    }

    CSVWriter(const std::string & filename){
        csvFile.open(filename, std::ios::out);
    }

    template <typename ... ColNames>
    CSVWriter(const std::string & filename, ColNames && ... cols) {
        csvFile.open(filename, std::ios::out);
        write(std::forward<ColNames>(cols)...);
    }

    auto operator = (CSVWriter && other) -> void {
        csvFile = std::move(other.csvFile);
    }

    template <typename Arg, typename ...Args>
    auto write(Arg&& arg, Args &&  ... args) -> void {
        std::lock_guard writeLock(m);
        csvFile << std::forward<Arg>(arg);
        ((csvFile << ',' << std::forward<Args>(args)), ...);
        csvFile << '\n';
    }
};