#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <iomanip>
#include <ios>
#include <fstream>
#include <cmath>

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

        void write_element(double element, std::streamsize precision, const string &suffix) {
            if (std::isnan(element))
                _output << "nan";
            else {
                const auto temp = _output.precision();
                _output.precision(precision);
                _output << std::fixed << element;
                _output.precision(temp);
            }

            _output << suffix;
        }

        void write_line(double time, double Hvar, double Evar, double Zvar) {
            write_element(time, 2, CDFCPP_SEPARATOR);
            write_element(Hvar, 2, CDFCPP_SEPARATOR);
            write_element(Evar, 2, CDFCPP_SEPARATOR);
            write_element(Zvar, 2, "\n");
        }

    private:
        path _filename;
        ofstream _output;
    };

}
