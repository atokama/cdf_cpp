#include <cdf_cpp/cdf_cpp.h>
#include <cdf_cpp/convertor.h>

using namespace cdf_cpp;

char *make_error_message(const std::string &text) {
    auto ret = static_cast<char *>(
            malloc(sizeof(char) * text.size()));
    if (ret) strcpy(ret, text.c_str());
    return ret;
}

const char *convertor(const char *source, const char *dest) try {
    Convertor::convert(source, dest);
    return nullptr;
} catch (const CDFError &e) {
    return make_error_message(e.what());
} catch (const std::exception &e) {
    return make_error_message(e.what());
} catch (...) {
    return make_error_message({"Unknown error"});
}

bool nasa_check(CDFFile& cdf, vector<string> vars, int nasa_secs) {
 return Convertor::is_subset(cdf.var_names(), vars)
         && cdf.variables.count("time") == 1
         && cdf.variables.at("time").num_elements == nasa_secs;
}

int nasa(char *error_message, const char *cdf_filename, int nasa_secs,
         double *t, double *hx, double *hy, double *hz) try {
    CDFFile cdf{cdf_filename};
    if (nasa_check(cdf, {"time", "HEZ"}, nasa_secs)) {
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = cdf.variables.at("time").element<double>(i, 0);
            hx[i] = cdf.variables.at("HEZ").element<double>(i, 0);
            hy[i] = cdf.variables.at("HEZ").element<double>(i, 1);
            hz[i] = cdf.variables.at("HEZ").element<double>(i, 2);
        }
    } else if (nasa_check(cdf, {"time", "Hvar20sec", "Evar20sec", "Zvar20sec"}, nasa_secs))
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = cdf.variables.at("time").element<double>(i, 0);
            hx[i] = cdf.variables.at("Hvar20sec").element<double>(i, 0);
            hy[i] = cdf.variables.at("Evar20sec").element<double>(i, 0);
            hz[i] = cdf.variables.at("Zvar20sec").element<double>(i, 0);
        }
    else if (nasa_check(cdf, {"time", "Hvar1min", "Evar1min", "Zvar1min"}, nasa_secs))
        for (auto i = 0L; i != nasa_secs; i += 1) {
            t[i] = cdf.variables.at("time").element<double>(i, 0);
            hx[i] = cdf.variables.at("Hvar1min").element<double>(i, 0);
            hy[i] = cdf.variables.at("Evar1min").element<double>(i, 0);
            hz[i] = cdf.variables.at("Zvar1min").element<double>(i, 0);
        }
    else {
        string err{ "Unexpected cdf file: "};
        if (cdf.variables.count("time"))
            err += "Length=" + std::to_string(
                        cdf.variables.at("time").num_elements) + " ";
        err += "Variables=[ " + Convertor::to_string(cdf.var_names()) + "]";
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
