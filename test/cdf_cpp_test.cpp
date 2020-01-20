#include <gtest/gtest.h>
#include <cdf.h>
#include <filesystem>

#define ASSERT_CDF_OK(x) ASSERT_EQ(CDF_OK, x)

namespace cdf_cpp {

    using std::cout;
    using std::endl;
    using std::string;

    TEST(cdf_cpp_test, a) {
        CDFid id;
        ASSERT_CDF_OK(CDFopenCDF("D:\\Mag\\proj\\cdf-cpp\\test\\data\\03\\ATU_20040301.cdf", &id));
        ASSERT_NE(nullptr, id);

        long TimeVarNum = CDFgetVarNum(id, "time");
        ASSERT_GE(TimeVarNum, CDF_WARN);

        long datatype;
        ASSERT_CDF_OK(CDFgetzVarDataType(id, TimeVarNum, &datatype));
        ASSERT_EQ(45, datatype);

        std::size_t ilen;
        ASSERT_CDF_OK(CDFgetDataTypeSize(datatype, &ilen));
        ASSERT_EQ(8, ilen);

        long numElements;
        ASSERT_CDF_OK(CDFgetzVarNumRecsWritten(id, TimeVarNum, &numElements));
        ASSERT_EQ(4320, numElements);

        ilen *= numElements;
        auto tv = std::string{};
        tv.resize(ilen, '\0');
        auto *time = (void *) tv.data();
        ASSERT_NE(nullptr, time);
        ASSERT_CDF_OK(CDFgetzVarRecordData(id, TimeVarNum, 1L, time));

        long indices[2];
        indices[0] = 0;
        indices[1] = 0;
        double TimeVarOut;
        ASSERT_CDF_OK(CDFgetzVarData(id, TimeVarNum, 0L, indices, &TimeVarOut));
        ASSERT_EQ(63245318400000, TimeVarOut);

        ASSERT_CDF_OK(CDFclose(id));
    }

}
