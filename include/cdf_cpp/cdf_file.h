#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <memory>

#include <cdf_cpp/variable.h>

#include <cdf.h>

namespace cdf_cpp {

    using std::string;
    using std::experimental::filesystem::path;
    template<typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;
    template <typename T> using sh_ptr = std::shared_ptr<T>;

    class CDFFile {
    public:
        explicit CDFFile(const path &filename);

        ~CDFFile();

        vector<string> var_names() const;

        map<string, sh_ptr<Variable>> variables;

        CDFid _id;
    };


}
