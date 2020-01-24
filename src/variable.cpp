#include <cdf_cpp/variable.h>

namespace cdf_cpp {

    Variable::Variable(CDFid id, long varnum)
            : _id{id}, _varnum{varnum} {
        check_status(CDFgetzVarNumRecsWritten(_id, _varnum, &num_elements), "num elements");
        check_status(CDFgetzVarDataType(_id, _varnum, &_datatype), "data type");
        SPDLOG_INFO("variable created: varnum={} datatype={} num_elements={}",
                    _varnum, _datatype, num_elements);
    }
}