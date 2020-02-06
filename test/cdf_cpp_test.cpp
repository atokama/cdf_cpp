#include <cdf_cpp/convertor.h>
#include <cdf_cpp/cdf_cpp.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>
#include <limits>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

namespace cdf_cpp {

    namespace fs = std::experimental::filesystem;

    using std::cout;
    using std::endl;
    using std::string;
    using std::ifstream;
    using fs::path;

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

    TEST_CASE("CDF datetime", "[cdf_cpp_test]") {
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

    TEST_CASE("cpp_extractor_type1", "[cdf_cpp_test]") {
        const auto cdf = "data\\03\\ATU_20040301.cdf";
        const auto txt = "data\\03\\ATU_20040301.txt";
        const auto temp = "ATU_20040301.txt";
        Convertor::convert(cdf, temp);
        REQUIRE(compare_files(temp, txt));
    }

    TEST_CASE("cpp_extractor_type2", "[cdf_cpp_test]") {
        const auto cdf = "data\\09\\ATU_19850910.cdf";
        const auto txt = "data\\09\\ATU_19850910.txt";
        const auto temp = "ATU_19850910.txt";
        Convertor::convert(cdf, temp);
        REQUIRE(compare_files(temp, txt));
    }

    TEST_CASE("cpp_extractor_type3", "[cdf_cpp_test]") {
        const auto cdf = "data\\10\\ATU_20161001.cdf";
        const auto txt = "data\\10\\ATU_20161001.txt";
        const auto temp = "ATU_20161001.txt";
        Convertor::convert(cdf, temp);
        REQUIRE(compare_files(temp, txt));
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
                    if (fs::is_regular_file(entry) && entry.path().extension() == ".cdf") {
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

}
