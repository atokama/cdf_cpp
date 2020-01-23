#pragma once

#include <filesystem>
#include <iomanip>
#include <map>
#include <string>
#include <fstream>

namespace cdf_cpp {

    using std::string;
    using std::ofstream;
    using std::experimental::filesystem::path;

    class TXTFile {
    public:
        explicit TXTFile(const path &filename, const string &separator, std::streamsize precision);

        ~TXTFile();

        void open();

        void close();

        void write_header();

        void write_line(double time, double Hvar, double Evar, double Zvar);

    private:
        void write_element(double element, const string &suffix);

        const path _filename;
        const string _separator;
        std::streamsize _precision;
        ofstream _output;
    };

}
