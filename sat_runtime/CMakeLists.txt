add_library(SAT_runtime)
target_sources(SAT_runtime PUBLIC FILE_SET CXX_MODULES FILES SAT_rt.cppm)
target_link_libraries(SAT_runtime CNF)

# Tests for library
add_executable(rt_sat tests.cpp)
target_link_libraries(rt_sat SAT_runtime CNF)
add_test(NAME Runtime_SAT
        COMMAND $<TARGET_FILE:rt_sat>)

INSTALL(TARGETS rt_sat)