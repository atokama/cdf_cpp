#pragma once

#include <filesystem>
#include <functional>
#include <sstream>
#include <fstream>

#include <spdlog/spdlog.h>

#include <cdf_cpp/cdf_file.h>
#include <cdf_cpp/variable.h>
#include <cdf_cpp/txt_file.h>

#define DLL_EXPORT extern "C" __declspec(dllexport)

namespace cdf_cpp {

    using std::endl;
    using std::string;
    using std::experimental::filesystem::path;
    template<typename T> using vector = std::vector<T>;
    template<typename K, typename T> using map = std::map<K, T>;

    string to_string(const vector<string> &v) {
        std::stringstream ss{};
        for (const auto &item : v)
            ss << item << " ";
        return ss.str();
    }

    bool is_subset(const vector<string> &first, const vector<string> &second) {
        SPDLOG_INFO("is subset?\n\tfirst:  {}\n\tsecond: {}", to_string(first), to_string(second));
        for (const auto &s: second)
            if (first.cend() == std::find(first.cbegin(), first.cend(), s))
                return false;
        return true;
    }

    void extract_type1(const map<string, Variable> &vars, long num_elements, TXTFile &txt) {
        for (auto i = 0L; i != num_elements; i += 3)
            txt.write_line(vars.at("time").element<double>(i),
                           vars.at("Hvar20sec").element<double>(i),
                           vars.at("Evar20sec").element<double>(i),
                           vars.at("Zvar20sec").element<double>(i));
    }

    void extract_type2(const map<string, Variable> &vars, long num_elements, TXTFile &txt) {
        for (auto i = 0L; i != num_elements; i += 1)
            txt.write_line(vars.at("time").element<double>(i),
                           vars.at("Hvar1min").element<double>(i),
                           vars.at("Evar1min").element<double>(i),
                           vars.at("Zvar1min").element<double>(i));
    }

    void cpp_extractor(const path &source, const path &dest) {
        CDFFile cdf{source};
        TXTFile txt{dest};
        std::function<
                void(const map<string, Variable> &, long, TXTFile &)
        > extr_ptr{nullptr};

        if (is_subset(cdf.var_names(), {"time", "Hvar20sec", "Evar20sec", "Zvar20sec"}))
            extr_ptr = &extract_type1;
        else if (is_subset(cdf.var_names(), {"time", "Hvar1min", "Evar1min", "Zvar1min"}))
            extr_ptr = &extract_type2;
        else
            throw CDFError{"is subset"};

        txt.open();
        txt.write_header();
        //todo assert variables all have same numElements
        const auto len = cdf.variables.at("time").num_elements;
        extr_ptr(cdf.variables, len, txt);
        txt.close();
    }


}

/**
 * @func Извлекает данные из .cdf файла и сохраняет в файл
 * назначения.
 * @ret NULL в случае успеха, иначе - описание ошибки.
 */
DLL_EXPORT
const char *
extractor(const char *source, const char *dest);
