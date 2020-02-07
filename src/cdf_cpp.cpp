#include <cdf_cpp/cdf_cpp.h>
#include <cdf_cpp/cdf_file.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace cdf_cpp;


bool is_subset(
        const vector<string> &first,
        const vector<string> &second) {
    for (const auto &s: second)
        if (first.cend() ==
                std::find(first.cbegin(), first.cend(), s))
            return false;
    return true;
}

string to_string(const vector<string> &v) {
    std::stringstream ss{};
    for (const auto &item : v)
        ss << item << " ";
    return ss.str();
}

int nasa(char *error_message, const char *cdf_filename, int length,
        double *t, double *hx, double *hy, double *hz);

int nasa_s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message, cdf_filename, NASA_S_NUM, t, hx, hy, hz);
}

int nasa_20s(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message,cdf_filename,  NASA_20S_NUM, t, hx, hy, hz);
}

int nasa_m(
        char *error_message,const char *cdf_filename,
        double *t, double *hx, double *hy, double *hz) {
    return nasa(error_message, cdf_filename, NASA_M_NUM, t, hx, hy, hz);
}


bool nasa_check(CDFFile& cdf, vector<string> vars, int nasa_secs) {
 return is_subset(cdf.var_names(), vars)
         && cdf.variables.count("time") == 1
         && cdf.variables.at("time")->num_elements == nasa_secs;
}

int nasa(char *error_message, const char *cdf_filename, int nasa_secs,
         double *t, double *hx, double *hy, double *hz) try {
    CDFFile cdf{cdf_filename};
    if (nasa_check(cdf, {"time", "HEZ"}, nasa_secs)) {
        auto tt = cdf.variables.at("time")->fast<double>(0);
        auto thx = cdf.variables.at("HEZ")->fast<double>(0);
        auto thy = cdf.variables.at("HEZ")->fast<double>(1);
        auto thz = cdf.variables.at("HEZ")->fast<double>(2);
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = tt[i]; hx[i] = thx[i];
            hy[i] = thy[i]; hz[i] = thz[i];
        }
    } else if (nasa_check(
                   cdf,  {"time", "Hvar20sec", "Evar20sec", "Zvar20sec"},
                   nasa_secs)) {
        auto tt = cdf.variables.at("time")->fast<double>(0);
        auto thx = cdf.variables.at("Hvar20sec")->fast<double>(0);
        auto thy = cdf.variables.at("Evar20sec")->fast<double>(0);
        auto thz = cdf.variables.at("Zvar20sec")->fast<double>(0);
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = tt[i]; hx[i] = thx[i];
            hy[i] = thy[i]; hz[i] = thz[i];
        }
    } else if (nasa_check(
                   cdf, {"time", "Hvar1min", "Evar1min", "Zvar1min"},
                   nasa_secs)) {
        auto tt = cdf.variables.at("time")->fast<double>(0);
        auto thx = cdf.variables.at("Hvar1min")->fast<double>(0);
        auto thy = cdf.variables.at("Evar1min")->fast<double>(0);
        auto thz = cdf.variables.at("Zvar1min")->fast<double>(0);
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = tt[i]; hx[i] = thx[i];
            hy[i] = thy[i]; hz[i] = thz[i];
        }
    } else {
        string err{ "Unexpected cdf file: "};
        if (cdf.variables.count("time"))
            err += "Length=" + std::to_string(
                        cdf.variables.at("time")->num_elements) + " ";
        err += "Variables=[ " + to_string(cdf.var_names()) + "]";
        strcpy(error_message, err.c_str());
        return 1;
    }
    return 0;
} catch (const CDFError &e) {
    strcpy(error_message, e.what());
    return 1;
} catch (const std::exception &e) {
    strcpy(error_message, e.what());
    return 1;
} catch (...) {
    strcpy(error_message, "Unknown error");
    return 1;
}

int test_string(char* str, int n) {
    std::string s{"from dll"};
    if (s.size() > n) return -1;
    strcpy(str, s.c_str());
    return 0;
}

double *test_array_2(int *size) {
    *size = 100;
    auto array = new double[*size];
    for (int i = 0; i != *size; ++i)
        array[i] = 1.5 * i;
    return array;
}


int test_array(double **array, int *size) {
    *size = 100;
    *array = new double[*size];
    for (int i = 0; i != *size; ++i)
        *((*array) + i) = 1.5 * i;
    return 0;
}

int dealloc_array(double *array) {
    delete[] array;
    return 0;
}

int test_array_3(double *array, int size) {
    for (int i = 0; i != size; ++i)
        array[i] = 1.5 * i;
    return 0;
}
