#include <cdf_cpp/cdf_cpp.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>
#include <limits>
#include <stdexcept>

//#include <easylogging++.h>
#include <gtest/gtest.h>

#include <cdf.h>

#define ASSERT_CDF_OK(x) ASSERT_EQ(CDF_OK, x)

namespace cdf_cpp {

    using std::cout;
    using std::endl;
    using std::string;
    using std::ifstream;
    using std::experimental::filesystem::path;


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

    constexpr auto file1_cdf = "D:\\Mag\\proj\\cdf-cpp\\test\\data\\03\\ATU_20040301.cdf";
    constexpr auto file1_txt = "D:\\Mag\\proj\\cdf-cpp\\test\\data\\03\\ATU_20040301.txt";
    constexpr auto temp_file = "ATU_20040301.txt";

    TEST(cdf_cpp_test, cpp_extractor) {
        cpp_extractor(file1_cdf, temp_file);
        ASSERT_TRUE(compare_files(temp_file, file1_txt));
    }

    TEST(cdf_cpp_test, a) {
        CDFid id;
        ASSERT_CDF_OK(CDFopenCDF(file1_cdf, &id));
        ASSERT_NE(nullptr, id);

        long TimeVarNum = CDFgetVarNum(id, "time");
        ASSERT_GE(TimeVarNum, CDF_WARN);

        long datatype;
        ASSERT_CDF_OK(CDFgetzVarDataType(id, TimeVarNum, &datatype));
        ASSERT_EQ(CDF_DOUBLE, datatype);

        std::size_t ilen;
        ASSERT_CDF_OK(CDFgetDataTypeSize(datatype, &ilen));
        ASSERT_EQ(8, ilen);

        long numElements;
        ASSERT_CDF_OK(CDFgetzVarNumRecsWritten(id, TimeVarNum, &numElements));
        ASSERT_EQ(4320, numElements);

        ilen *= numElements;
        auto tv = string{};
        tv.resize(ilen, '\0');
        auto p = static_cast<void *>(const_cast<char *>(tv.data()));
        ASSERT_CDF_OK(CDFgetzVarRecordData(id, TimeVarNum, 1L, p));

        long indices[2];
        indices[0] = 0;
        indices[1] = 0;
        double result;

        auto cast = [](double x) { return static_cast<long long>(x); };

        ASSERT_CDF_OK(CDFgetzVarData(id, TimeVarNum, 0L, indices, &result));
        ASSERT_EQ(63245318400000, cast(result));

        ASSERT_CDF_OK(CDFgetzVarData(id, TimeVarNum, 3L, indices, &result));
        ASSERT_EQ(63245318460000LL, cast(result));

        // out of range
        ASSERT_EQ(1001, CDFgetzVarData(id, TimeVarNum, numElements, indices, &result));

        ASSERT_CDF_OK(CDFclose(id));
    }

    TEST(cdf_cpp_test, b) {
        CDFid id;
        ASSERT_CDF_OK(CDFopenCDF(file1_cdf, &id));
        ASSERT_NE(nullptr, id);

        long numzVars;
        ASSERT_CDF_OK(CDFgetNumzVars(id, &numzVars));
        ASSERT_EQ(16, numzVars);

        char buff[256];
        for (long index = 0L; index != numzVars; ++index) {
            ASSERT_CDF_OK(CDFgetzVarName(id, index, buff));
            cout << buff << endl;
        }
    }

}
