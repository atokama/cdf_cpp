#pragma once

#include <filesystem>
#include <map>
#include <string>

#include <spdlog/spdlog.h>

#include <cdf_cpp/cdf_error.h>

#define CDFCPP_SEPARATOR "\t"

namespace cdf_cpp {

    using std::ofstream;
    using std::endl;
    using std::experimental::filesystem::path;

    class TXTFile {
    public:
        explicit TXTFile(const path &filename) : _filename{filename}, _output{} {
        }

        ~TXTFile() {
            if (_output.is_open()) close();
            SPDLOG_INFO("txt file is closed, filename: {}", _filename.string());
        }


        void open() {
            _output.open(_filename.string(), ofstream::out);
            SPDLOG_INFO("txt file is open, filename: {}", _filename.string());
        }

        void close() {
            _output.close();
        }

        void write_header() {
            _output << "time" << CDFCPP_SEPARATOR
                    << "Hvar" << CDFCPP_SEPARATOR
                    << "Evar" << CDFCPP_SEPARATOR
                    << "Zvar" << endl;
        }

        void write_line(double time, double Hvar, double Evar, double Zvar) {
            auto cast = [](double x) { return static_cast<long long>(x); };
            _output << cast(time) << CDFCPP_SEPARATOR
                    << cast(Hvar) << CDFCPP_SEPARATOR
                    << cast(Evar) << CDFCPP_SEPARATOR
                    << cast(Zvar) << endl;
        }

    private:
        path _filename;
        ofstream _output;
    };

}
