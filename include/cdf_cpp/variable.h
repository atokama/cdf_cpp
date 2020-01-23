#pragma once

#include <string>

#include <spdlog/spdlog.h>

#include <cdf.h>

#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    using std::string;

    class Variable {
    public:
        Variable(CDFid id, long varnum);

        template<typename T>
        T element(long index) const {
            if (index >= num_elements) throw CDFError{"index >= length"};
            T result;
            check_status(CDFgetzVarData(_id, _varnum, index, _indices, &result), "get var data");
            return result;
        }

        long num_elements;
    private:
        CDFid _id;
        long _varnum;
        long _datatype;
        long _indices[2];
    };
}
