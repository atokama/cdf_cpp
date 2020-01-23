#pragma once

#include <stdexcept>
#include <string>

#include <cdf.h>

namespace cdf_cpp {

    using std::string;

    class CDFError : std::runtime_error {
    public:
        explicit CDFError(const string& m);

        const char *what() const noexcept;
    };

    void check_status(CDFstatus status, const string &error_message);
}
