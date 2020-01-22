#pragma once

#include <stdexcept>
#include <string>

#include <cdf.h>

namespace cdf_cpp {

    using std::string;

    class CDFError : std::runtime_error {
    public:
        explicit CDFError(const string& m) : std::runtime_error{m} {}

        const char *what() const noexcept { return "err"; }
    };

    void check_status(CDFstatus status, const string &error_message) {
        if (status != CDF_OK) throw CDFError{error_message};
    }
}
