#include <cdf_cpp/convertor.h>

#include <functional>
#include <sstream>

namespace cdf_cpp {

    bool Convertor::is_subset(
            const vector<string> &first,
            const vector<string> &second) {
        for (const auto &s: second)
            if (first.cend() ==
                    std::find(first.cbegin(), first.cend(), s))
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
