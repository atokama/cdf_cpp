set(CMAKE_GENERATOR_PLATFORM x86)

function(cdf_sample n)
    add_executable(${n} ${n}.c)
    target_link_libraries(${n} cdf)
endfunction()

cdf_sample(example1)
cdf_sample(OperateAttributes)
cdf_sample(OperateCDF)
cdf_sample(OperateVariables)
cdf_sample(read_example1)
