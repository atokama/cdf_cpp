cmake_minimum_required(VERSION 3.12)
project(cdf-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

# https://spdf.gsfc.nasa.gov/pub/software/cdf/dist/cdf37_1/cdf-dist-all.tar.gz
set(CDF_DISTRIBUTION_DIR "cdf37_1-dist")

file(GLOB s 
	"${CDF_DISTRIBUTION_DIR}/lib/*.c" 
	"${CDF_DISTRIBUTION_DIR}/lib/zlib/*.c"
	"src/*.cpp")

list(REMOVE_ITEM s "${CDF_DISTRIBUTION_DIR}/lib/bsearcho.c")

add_library(cdf_cpp SHARED ${s})

target_include_directories(cdf_cpp PUBLIC include)
target_compile_definitions(cdf_cpp PUBLIC _FILE_OFFSET_BITS=64)
set_target_properties(cdf_cpp PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)

# test
file(COPY test/data DESTINATION .)
add_executable(cdf_cpp_test test/cdf_cpp_test.cpp)
target_link_libraries(cdf_cpp_test cdf_cpp)

# cdf distribution test
#
#enable_testing()
#function(cdf_test n)
#    add_executable(${n} tests/${n}.c)
#    target_link_libraries(${n} cdf)
#    add_test(NAME ${n} COMMAND ${n})
#endfunction()
#
#cdf_test(qst2c)
#cdf_test(qst2cOrig)
#cdf_test(qst2ic)
#cdf_test(qst2icEPOCH16)
#cdf_test(qst2icTT2000)
#cdf_test(qst2Version)
