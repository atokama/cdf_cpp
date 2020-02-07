#pragma once

#include <string>
#include <vector>

#include <cdf.h>
#include <cdf_cpp/cdf_error.h>

namespace cdf_cpp {

    using std::string;
    template <typename T> using vector = std::vector<T>;

    class Variable {
    public:
        Variable(CDFid id, long varnum);
        ~Variable();

        /*
        template<typename T>
        T element(long row, long column) const {
            if (row >= num_elements)
                throw CDFError{"row >= num_elements"};
            T result;
            long indices[] = {column};
            check_status(CDFgetzVarData(_id, _varnum, row, indices, &result),
                         "get var data");
            return result;
        }
        */

        long num_columns() const { return _num_columns; }

        template<typename T>
        vector<T> fast(long column) const {
            if (column >= _num_columns)
                throw CDFError{"column >= num_columns"};

            vector<T> ret;
            ret.resize(_num_rows);
            for (auto i=0; i != _num_rows; i++)
                *(ret.data() + i) = *(((T *)_image.data())+i*_num_columns+column);
            return ret;
        }

        long num_elements;
    private:
        CDFid _id;
        long _varnum;
        long _datatype;

        vector<char> _image;
        long _num_columns, _num_rows;
    };
}
