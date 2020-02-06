#include <cdf_cpp/txt_file.h>

#include <cmath>
#include <ios>

#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    using std::endl;

    TXTFile::TXTFile(const path &filename, const string &separator, std::streamsize precision)
        : _filename{filename}, _separator{separator}, _precision{precision}, _output{} {
    }

    TXTFile::~TXTFile() {
        if (_output.is_open()) close();
    }

    void TXTFile::open() {
        _output.open(_filename.string(), ofstream::out);
        if (!_output.is_open()) throw CDFError{"fail to create .txt file " + _filename.string()};
    }

    void TXTFile::close() {
        _output.close();
    }

    void TXTFile::write_header() {
        _output << "time" << _separator
                << "Hvar" << _separator
                << "Evar" << _separator
                << "Zvar" << endl;
    }

    void TXTFile::write_element(double element, const string &suffix) {
        if (std::isnan(element))
            _output << "nan";
        else {
            const auto temp = _output.precision();
            _output.precision(_precision);
            _output << std::fixed << element;
            _output.precision(temp);
        }

        _output << suffix;
    }

    void TXTFile::write_line(double time, double Hvar, double Evar, double Zvar) {
        write_element(time, _separator);
        write_element(Hvar, _separator);
        write_element(Evar, _separator);
        write_element(Zvar, "\n");
    }
}
