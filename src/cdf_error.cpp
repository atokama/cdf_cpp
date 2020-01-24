#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    CDFError::CDFError(const string &error_message) : std::runtime_error{error_message} {}

    void check_status(CDFstatus status, const string &error_message) {
        if (status != CDF_OK) throw CDFError{error_message};
    }
}
