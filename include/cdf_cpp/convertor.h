#pragma once

#include <filesystem>
#include <fstream>

#include <cdf_cpp/cdf_file.h>
#include <cdf_cpp/txt_file.h>

namespace cdf_cpp {

    using std::string;
    using std::experimental::filesystem::path;
    template<typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;

    class Convertor {
    public:
        static void convert(const path &source, const path &dest);

    private:
        static void extract_type1(const map<string, Variable> &vars, long num_elements, TXTFile &txt);

        static void extract_type2(const map<string, Variable> &vars, long num_elements, TXTFile &txt);

        static void extract_type3(const map<string, Variable> &vars, long num_elements, TXTFile &txt);

        static bool is_subset(const vector<string> &first, const vector<string> &second);

        static string to_string(const vector<string> &v);

    };

}