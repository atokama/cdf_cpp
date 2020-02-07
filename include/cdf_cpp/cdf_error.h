#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <cdf.h>

namespace cdf_cpp {

    template <typename T> using sh_ptr = std::shared_ptr<T>;
    template <typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;
    template<typename T> using vector = std::vector<T>;
    using std::experimental::filesystem::path;
    using std::string;

    class CDFError : public std::runtime_error {
    public:
        explicit CDFError(const string &error_message);
    };

    void check_status(CDFstatus status, const string &error_message);
}
