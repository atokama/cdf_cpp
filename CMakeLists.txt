cmake_minimum_required(VERSION 3.15)
project(clion1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

#add_subdirectory(../cdf37_1-dist cdf371)

add_library(clion1 SHARED main.cpp)
#target_link_libraries(clion1 cdf)
set_target_properties(clion1 PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

add_executable(clion2 main.cpp)
#target_link_libraries(clion1 cdf2txt)