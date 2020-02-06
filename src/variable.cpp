#include <cdf_cpp/variable.h>

namespace cdf_cpp {

    Variable::Variable(CDFid id, long varnum)
            : _id{id}, _varnum{varnum} {
        check_status(CDFgetzVarNumRecsWritten(_id, _varnum, &num_elements),
                     "fail to get number of variable elements");
        check_status(CDFgetzVarDataType(_id, _varnum, &_datatype),
                     "fail to get datatype of variable");
    }
}
