#pragma once

#include <cdf_cpp/cdf_error.h>
#include <cdf_cpp/variable.h>

#include <cdf.h>

namespace cdf_cpp {

    class CDFFile {
    public:
        explicit CDFFile(const path &filename);

        ~CDFFile();

        vector<string> var_names() const;

        map<string, sh_ptr<Variable>> variables;

        CDFid _id;
    };


}
