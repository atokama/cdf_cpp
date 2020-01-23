#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    CDFError::CDFError(const string &m) : std::runtime_error{m} {}

    const char *CDFError::what() const noexcept { return "err"; }

    void check_status(CDFstatus status, const string &error_message) {
        if (status != CDF_OK) throw CDFError{error_message};
    }
}
