#pragma once

#include <filesystem>
#include <fstream>

#include <cdf_cpp/cdf_file.h>

namespace cdf_cpp {

    using std::string;
    using std::experimental::filesystem::path;
    template<typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;

    class Convertor {
    public:
        static bool is_subset(
                const vector<string> &first,
                const vector<string> &second);

        static string to_string(const vector<string> &v);
    };

}
