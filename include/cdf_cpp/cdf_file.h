#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include <cdf_cpp/variable.h>

#include <cdf.h>

namespace cdf_cpp {

    using std::string;
    using std::experimental::filesystem::path;
    template<typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;

    class CDFFile {
    public:
        explicit CDFFile(const path &filename);

        ~CDFFile();

        vector<string> var_names() const;

        map<string, Variable> variables;

    private:
        CDFid _id;
    };


}
