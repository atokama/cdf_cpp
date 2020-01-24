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
