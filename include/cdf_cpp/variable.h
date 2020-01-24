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
        T element(long row, long column) const {
            if (row >= num_elements) throw CDFError{"row >= num_elements"};
            T result;
            long indices[] = {column};
            check_status(CDFgetzVarData(_id, _varnum, row, indices, &result), "get var data");
            return result;
        }

        long num_elements;
    private:
        CDFid _id;
        long _varnum;
        long _datatype;
    };
}
