cmake_minimum_required(VERSION 3.12)
project(cdf-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

add_subdirectory(../cdf37_1-dist cdf371)

add_library(cdf_cpp SHARED src/cdf_cpp.cpp src/cdf_error.cpp src/cdf_file.cpp
        src/txt_file.cpp src/variable.cpp src/convertor.cpp)
target_include_directories(cdf_cpp PUBLIC include)
target_link_libraries(cdf_cpp cdf)
set_target_properties(cdf_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

file(COPY test/data DESTINATION .)
add_executable(cdf_cpp_test test/cdf_cpp_test.cpp)
target_link_libraries(cdf_cpp_test cdf_cpp)
