#include <cdf_cpp/cdf_file.h>

#include <spdlog/spdlog.h>

namespace cdf_cpp {

    CDFFile::CDFFile(const path &filename) : _id{nullptr} {
        check_status(CDFopenCDF(filename.string().c_str(), &_id), "open CDF");

        long numzVars;
        check_status(CDFgetNumzVars(_id, &numzVars), "num zVars");

        SPDLOG_INFO("num zVars: {}", numzVars);
        for (long i = 0L; i != numzVars; ++i) {
            char var_name[CDF_VAR_NAME_LEN256 + 1];
            check_status(CDFgetzVarName(_id, i, var_name), "zVar name");

            long var_num = CDFgetVarNum(_id, var_name);
            if (var_num < CDF_WARN) throw CDFError{"var num"};

            string n{var_name};
            variables.insert({n, Variable{_id, var_num}});
            SPDLOG_INFO("inserted var: {} varnum: {} total vars: {}", n, var_num, variables.size());
        };
    }

    CDFFile::~CDFFile() {
        if (_id) CDFclose(_id);
    }

    vector<string> CDFFile::var_names() const {
        vector<string> vars{};
        for (const auto &pair: variables) vars.push_back(pair.first);
        return vars;
    }
}
