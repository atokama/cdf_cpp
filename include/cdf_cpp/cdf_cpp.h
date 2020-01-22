#pragma once

#include <filesystem>
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

    string toString(const vector<string> &v) {
        std::stringstream ss{};
        for (const auto &item : v)
            ss << item << " ";
        return ss.str();
    }

    bool isSubset(const vector<string> &first, const vector<string> &second) {
        SPDLOG_INFO("is subset?\n\tfirst:  {}\n\tsecond: {}", toString(first), toString(second));
        for (const auto &s: second)
            if (first.cend() == std::find(first.cbegin(), first.cend(), s))
                return false;
        return true;
    }

    void extract_set1(const CDFFile &cdf, TXTFile &txt) {
        txt.open();
        txt.write_header();

        const auto &vars = cdf.variables;
        //todo assert variables all have same numElements
        const auto len = vars.at("time").length;
        for (auto i = 0L; i != len; i += 3)
            txt.write_line(vars.at("time").element<double>(i),
                           vars.at("Hvar20sec").element<double>(i),
                           vars.at("Evar20sec").element<double>(i),
                           vars.at("Zvar20sec").element<double>(i));
        txt.close();
    }

    void cpp_extractor(const path &source, const path &dest) {
        CDFFile cdf{source};
        TXTFile txt{dest};
        vector<string> vars{};
        for (const auto &pair: cdf.variables)
            vars.push_back(pair.first);

        if (isSubset(vars, {"time", "Hvar20sec", "Evar20sec", "Zvar20sec"}))
            extract_set1(cdf, txt);
        else
            throw CDFError{""};
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
