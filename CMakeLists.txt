cmake_minimum_required(VERSION 3.12)
project(cdf-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_GENERATOR_PLATFORM x86)

if (MSVC)
	set(CMAKE_SUPPRESS_REGENERATION ON)
endif()

# https://spdf.gsfc.nasa.gov/pub/software/cdf/dist/cdf37_1/cdf-dist-all.tar.gz
set(CDF_DISTRIBUTION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cdf37_1-dist")

file(GLOB cdf_dist RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	"${CDF_DISTRIBUTION_DIR}/src/lib/*.c" 
	"${CDF_DISTRIBUTION_DIR}/src/lib/zlib/*.c")
list(REMOVE_ITEM cdf_dist "${CDF_DISTRIBUTION_DIR}/lib/bsearcho.c")
source_group("CDF Distribution" FILES ${cdf_dist})

file(GLOB incl RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/cdf_cpp/*.h)
source_group("Include Files" FILES ${incl})

file(GLOB src RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
source_group("Source Files" FILES ${src})


add_library(cdf_cpp SHARED ${cdf_dist} ${incl} ${src})

target_include_directories(cdf_cpp PUBLIC 
	"include" "${CDF_DISTRIBUTION_DIR}/src/include")

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
