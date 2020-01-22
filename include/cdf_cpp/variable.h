#pragma once

#include <string>

#include <spdlog/spdlog.h>

#include <cdf.h>

#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    using std::string;

    class Variable {
    public:
        Variable(CDFid id, long varnum) : _id{id}, _varnum{varnum} {
            _indices[0] = 0;
            _indices[1] = 0;
            check_status(CDFgetzVarNumRecsWritten(_id, _varnum, &length), "num elements");
            check_status(CDFgetzVarDataType(_id, _varnum, &_datatype), "data type");
            SPDLOG_INFO("ctor varnum: {} datatype: {} numrecs: {}", _varnum, _datatype, length);
        }
        template<typename T>
        T element(long index) const {
            if (index >= length) throw CDFError{"index >= length"};
            T result;
            check_status(CDFgetzVarData(_id, _varnum, index, _indices, &result), "get var data");
            SPDLOG_INFO("element varnum: {} index: {} value: {}", _varnum, index, result);
            return result;
        }

        long length;
    private:
        long _indices[2];
        CDFid _id;
        long _varnum;
        long _datatype;
    };
}
