cmake_minimum_required(VERSION 3.15)
project(cdf-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

add_subdirectory(../cdf37_1-dist cdf371)

add_library(clion1 SHARED main.cpp)
target_include_directories(clion1 PUBLIC include)
set_target_properties(clion1 PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

add_library(cdf_cpp SHARED src/cdf_cpp.cpp)
target_include_directories(cdf_cpp PUBLIC include)
target_link_libraries(cdf_cpp cdf)
set_target_properties(cdf_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

find_package(Threads REQUIRED)
add_subdirectory(../googletest googletest)

add_executable(cdf_cpp_test test/gtest_main.cpp test/cdf_cpp_test.cpp)
target_link_libraries(cdf_cpp_test cdf_cpp ${CMAKE_THREAD_LIBS_INIT} gtest)
