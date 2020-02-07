#include <cdf_cpp/cdf_cpp.h>
#include <cdf_cpp/cdf_file.h>

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>
#include <limits>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

namespace cdf_cpp {

    namespace fs = std::experimental::filesystem;
    using fs::path;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ofstream;

    class TXTFile {
    public:
        explicit TXTFile(
                const path &filename,
                const string &separator,
                std::streamsize precision);
        ~TXTFile();

        void write_header();
        void write_line(double time, double Hvar, double Evar, double Zvar);

    private:
        void close();
        void open();
        void write_element(double element, const string &suffix);
        const path _filename;
        const string _separator;
        std::streamsize _precision;
        ofstream _output;
    };

    TXTFile::TXTFile(const path &filename,
                     const string &separator,
                     std::streamsize precision)
        : _filename{filename}, _separator{separator},
          _precision{precision}, _output{} {
        open();
    }

    TXTFile::~TXTFile() {
        if (_output.is_open()) close();
    }

    void TXTFile::open() {
        _output.open(_filename.string(), ofstream::out);
        if (!_output.is_open())
            throw CDFError{"fail to create .txt file "
                           + _filename.string()};
    }

    void TXTFile::close() { _output.close(); }

    void TXTFile::write_header() {
        _output << "time" << _separator
                << "Hvar" << _separator
                << "Evar" << _separator
                << "Zvar" << endl;
    }

    void TXTFile::write_element(double element,
                                const string &suffix) {
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

    void TXTFile::write_line(
            double time,
            double Hvar, double Evar, double Zvar) {
        write_element(time, _separator);
        write_element(Hvar, _separator);
        write_element(Evar, _separator);
        write_element(Zvar, "\n");
    }

    template<typename InputIterator1, typename InputIterator2>
    bool range_equal(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2) {
        while (first1 != last1 && first2 != last2) {
            if (*first1 != *first2) return false;
            ++first1;
            ++first2;
        }
        return (first1 == last1) && (first2 == last2);
    }

    bool compare_files(const string &filename1, const string &filename2) {
        ifstream file1(filename1);
        ifstream file2(filename2);

        std::istreambuf_iterator<char> begin1(file1);
        std::istreambuf_iterator<char> begin2(file2);

        std::istreambuf_iterator<char> end;

        return range_equal(begin1, end, begin2, end);
    }

    void save_txt_file(const path &filename, int length,
                       double *t, double *hx, double *hy, double *hz) {
        TXTFile txt{filename, "\t", 2};
        txt.write_header();
        for (int i = 0; i != length; ++i)
            txt.write_line(*t++, *hx++, *hy++, *hz++);
    }

    bool nasa_tester(int len,
                     const char *cdf, const char *txt, const char *temp) {
        char error_message[NASA_ERROR_SIZE];
        error_message[0] = '\0';
        std::vector<double> t(len), hx(len), hy(len), hz(len);
        nasa(error_message, cdf, len, t.data(), hx.data(), hy.data(), hz.data());
        save_txt_file(temp, len, t.data(), hx.data(), hy.data(), hz.data());
        return compare_files(temp, txt);
    }

    TEST_CASE("nasa_s", "[cdf_cpp_test]") {
        const auto cdf = "data\\10\\ATU_20161001.cdf";
        const auto txt = "data\\10\\ATU_20161001.txt";
        const auto temp = "ATU_20161001.txt";
        REQUIRE(nasa_tester(NASA_S_NUM, cdf, txt, temp));
    }

    TEST_CASE("nasa_20s", "[cdf_cpp_test]") {
        const auto cdf = "data\\03\\ATU_20040301.cdf";
        const auto txt = "data\\03\\ATU_20040301.txt";
        const auto temp = "ATU_20040301.txt";
        REQUIRE(nasa_tester(NASA_20S_NUM, cdf, txt, temp));
    }

    TEST_CASE("nasa_m", "[cdf_cpp_test]") {
        const auto cdf = "data\\09\\ATU_19850910.cdf";
        const auto txt = "data\\09\\ATU_19850910.txt";
        const auto temp = "ATU_19850910.txt";
        REQUIRE(nasa_tester(NASA_M_NUM, cdf, txt, temp));
    }

    std::vector<double> get_time_var(
            const char* cdf_filename, int length) {
        char error_message[NASA_ERROR_SIZE];
        error_message[0] = '\0';
        std::vector<double> t(length), hx(length), hy(length), hz(length);

        nasa_s(error_message, cdf_filename, t.data(), hx.data(), hy.data(), hz.data());
        return t;
    }

    TEST_CASE("epoch_time") {
        const auto cdf_s = "data\\10\\ATU_20161001.cdf";
        auto t = get_time_var(cdf_s, NASA_S_NUM);

        char epoch[1024];
        epoch[0] = '\0';

        toEncodeEPOCH(t[0], 0, epoch);
        REQUIRE("01-Oct-2016 00:00:00.000" ==  string{epoch});
        toEncodeEPOCH(t[0], 1, epoch);
        REQUIRE("20161001.0000000" ==  string{epoch});
        toEncodeEPOCH(t[0], 2, epoch);
        REQUIRE("20161001000000" ==  string{epoch});
        toEncodeEPOCH(t[0], 3, epoch);
        REQUIRE("2016-10-01T00:00:00.000Z" ==  string{epoch});
        toEncodeEPOCH(t[0], 4, epoch);
        REQUIRE("2016-10-01T00:00:00.000" ==  string{epoch});

        SECTION("unixtime") {
            double unix;
            EPOCHtoUnixTime(t.data() + 100, &unix, 1);
            REQUIRE(1475280100. == unix);
        }
    }

    string nasa_error_tester(int len, const char *cdf) {
        auto error_message = new char[NASA_ERROR_SIZE];
        error_message[0] = '\0';
        std::vector<double> t(len), hx(len), hy(len), hz(len);
        nasa(error_message, cdf, len,
             t.data(), hx.data(), hy.data(), hz.data());
         string ret{error_message};
         delete [] error_message;
         return ret;
    }

    TEST_CASE("nasa_mix_errors", "[cdf_cpp_test]") {
        const auto cdf_s = "data\\10\\ATU_20161001.cdf";
        const auto cdf_20s = "data\\03\\ATU_20040301.cdf";
        const auto cdf_m = "data\\09\\ATU_19850910.cdf";

        // no error: nasa_s called with s file
        REQUIRE("" ==
                nasa_error_tester(NASA_S_NUM, cdf_s));

        // nasa_s called with 20s file
        REQUIRE("Unexpected cdf file: Length=4320 Variables=[ DT "
                "Evar20sec FB FS HEqla HEscv HNqla HNscv Hvar20sec "
                "IN SD T20sec Zqla Zscv Zvar20sec time ]" ==
                nasa_error_tester(NASA_S_NUM, cdf_20s));

        // nasa_s called with m file
        REQUIRE("Unexpected cdf file: Length=1440 Variables=[ Evar1min "
                "HEqla HNqla Hvar1min Zqla Zvar1min time ]" ==
                nasa_error_tester(NASA_S_NUM, cdf_m));

        // nasa_20s called with s file
        REQUIRE("Unexpected cdf file: Length=86400 Variables=[ HEZ time ]" ==
                nasa_error_tester(NASA_20S_NUM, cdf_s));

        // nasa_20s called with 20s file
        REQUIRE("" ==
                nasa_error_tester(NASA_20S_NUM, cdf_20s));

        // nasa_20s called with m file
        REQUIRE("Unexpected cdf file: Length=1440 Variables=[ Evar1min "
                "HEqla HNqla Hvar1min Zqla Zvar1min time ]" ==
                nasa_error_tester(NASA_20S_NUM, cdf_m));

        // nasa_m called with s file
        REQUIRE("Unexpected cdf file: Length=86400 Variables=[ HEZ time ]" ==
                nasa_error_tester(NASA_M_NUM, cdf_s));

        // nasa_m called with 20s file
        REQUIRE("Unexpected cdf file: Length=4320 Variables=[ DT Evar20sec "
                "FB FS HEqla HEscv HNqla HNscv Hvar20sec IN SD T20sec Zqla "
                "Zscv Zvar20sec time ]" ==
                nasa_error_tester(NASA_M_NUM, cdf_20s));

        // nasa_m called with m file
        REQUIRE("" ==
                nasa_error_tester(NASA_M_NUM, cdf_m));
    }

    TEST_CASE("not_a_nasa_file", "[cdf_cpp_test]") {
        auto cdf = "data\\example1.cdf";
        REQUIRE("Unexpected cdf file: Variables=[ Image Latitude Time ]" ==
                nasa_error_tester(NASA_S_NUM, cdf));
    }

    TEST_CASE("bad_file_extension", "[cdf_cpp_test]") {
        auto cdf = "data\\example.dat";
        REQUIRE("fail to open CDF file" ==
                nasa_error_tester(NASA_S_NUM, cdf));
    }

    TEST_CASE("file_not_found", "[cdf_cpp_test]") {
        auto cdf = "file_not_exist";
        REQUIRE("fail to open CDF file" ==
                nasa_error_tester(NASA_S_NUM, cdf));
    }

    /*
    TEST_CASE("all_files", "[cdf_cpp_test]") {
        const path data_dir{"data"}, temp_dir{"temp"};

                // for each subdir in data_dir
        for (auto &subdir : fs::directory_iterator{data_dir})
            if (fs::is_directory(subdir)) {
                const auto output_dir = temp_dir / subdir.path().filename();
                fs::create_directories(output_dir);

                                // for each file in subdir
                for (auto &entry : fs::directory_iterator{subdir.path()})
                    if (fs::is_regular_file(entry)
                        && entry.path().extension() == ".cdf") {
                        auto txt = entry.path();
                        txt.replace_extension(".txt");
                        const auto temp = output_dir / txt.filename();

                                                // convert to txt file
                        Convertor::convert(entry.path(), temp);
                        REQUIRE(compare_files(temp.string(), txt.string()));
                    }
            }

    }
    */


    /*
    TEST_CASE("Test arrays", "[cdf_cpp_test]") {
        int i;
        double *pd2 = test_array_2(&i);
        REQUIRE(i == 100);
        REQUIRE(pd2[1] == 1.5);

        double *pd;
        test_array(&pd, &i);
        REQUIRE(pd[2] == 3);

        double *pd3 = new double[100];
        test_array_3(pd3, 100);
        delete[] pd3;
    }
    */
}
