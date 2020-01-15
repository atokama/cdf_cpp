#include <iostream>
#include <string>
#include <algorithm>

#define DLL_EXPORT extern "C" __declspec(dllexport)

DLL_EXPORT int add_two(int a, int b) {
    //std::cout << "Hello, World!" << std::endl;
    return a + b;
}

DLL_EXPORT int char_size() {
    return sizeof(char);
}

DLL_EXPORT int do_char(char *c) {
    return (int) *c;
}

DLL_EXPORT int str_len(const char*s1) {
    return strlen(s1);
//    return std::string{s1}.size();
}

DLL_EXPORT const char *do_str(const char *c) {
    auto s = new std::string{c};
    std::sort(s->begin(), s->end());
    return s->c_str();
}

int main() {
    std::cout << "test //\\\\" << std::endl;
}

