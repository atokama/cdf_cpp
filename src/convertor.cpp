#include <cdf_cpp/convertor.h>

#include <functional>
#include <sstream>

#include <spdlog/spdlog.h>

namespace cdf_cpp {

    void Convertor::convert(const path &source, const path &dest) {
        CDFFile cdf{source};
        TXTFile txt{dest, "\t", 2};
        std::function<
                void(const map<string, Variable> &, long, TXTFile &)
        > extr_ptr{nullptr};

        if (is_subset(cdf.var_names(), {"time", "Hvar20sec", "Evar20sec", "Zvar20sec"}))
            extr_ptr = &extract_type1;
        else if (is_subset(cdf.var_names(), {"time", "Hvar1min", "Evar1min", "Zvar1min"}))
            extr_ptr = &extract_type2;
        else if (is_subset(cdf.var_names(), {"time", "HEZ"}))
            extr_ptr = &extract_type3;
        else
            throw CDFError{"file does not contain necessary variables"};

        txt.open();
        txt.write_header();
        //todo assert variables all have same numElements
        const auto len = cdf.variables.at("time").num_elements;
        extr_ptr(cdf.variables, len, txt);
        txt.close();
    }

    void Convertor::extract_type1(const map<string, Variable> &vars, long num_elements, TXTFile &txt) {
        for (auto i = 0L; i != num_elements; i += 3)
            txt.write_line(vars.at("time").element<double>(i, 0),
                           vars.at("Hvar20sec").element<double>(i, 0),
                           vars.at("Evar20sec").element<double>(i, 0),
                           vars.at("Zvar20sec").element<double>(i, 0));
    }

    void Convertor::extract_type2(const map<string, Variable> &vars, long num_elements, TXTFile &txt) {
        for (auto i = 0L; i != num_elements; i += 1)
            txt.write_line(vars.at("time").element<double>(i, 0),
                           vars.at("Hvar1min").element<double>(i, 0),
                           vars.at("Evar1min").element<double>(i, 0),
                           vars.at("Zvar1min").element<double>(i, 0));
    }

    void Convertor::extract_type3(const map<string, Variable> &vars, long num_elements, TXTFile &txt) {
        for (auto i = 0L; i != num_elements; i += 60)
            txt.write_line(vars.at("time").element<double>(i, 0),
                           vars.at("HEZ").element<double>(i, 0),
                           vars.at("HEZ").element<double>(i, 1),
                           vars.at("HEZ").element<double>(i, 2));
    }

    bool Convertor::is_subset(const vector<string> &first, const vector<string> &second) {
        for (const auto &s: second)
            if (first.cend() == std::find(first.cbegin(), first.cend(), s))
                return false;
        return true;
    }

    string Convertor::to_string(const vector<string> &v) {
        std::stringstream ss{};
        for (const auto &item : v)
            ss << item << " ";
        return ss.str();
    }
}
