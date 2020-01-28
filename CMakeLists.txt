cmake_minimum_required(VERSION 3.12)
project(cdf-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

add_subdirectory(../cdf37_1-dist cdf371)
add_subdirectory(../spdlog spdlog)

add_library(cdf_cpp SHARED src/cdf_cpp.cpp src/cdf_error.cpp src/cdf_file.cpp
        src/txt_file.cpp src/variable.cpp src/convertor.cpp)
target_include_directories(cdf_cpp PUBLIC include)
target_link_libraries(cdf_cpp spdlog cdf)
set_target_properties(cdf_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)
target_compile_definitions(cdf_cpp PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF)

find_package(Threads REQUIRED)
add_subdirectory(../googletest googletest)

file(COPY test/data DESTINATION .)
add_executable(cdf_cpp_test test/gtest_main.cpp test/cdf_cpp_test.cpp)
target_link_libraries(cdf_cpp_test cdf_cpp ${CMAKE_THREAD_LIBS_INIT} gtest)
