# Tests for library
add_library(SAT_hybrid)
target_sources(SAT_hybrid PUBLIC FILE_SET CXX_MODULES FILES SAT_hyb.cppm)
target_link_libraries(SAT_hybrid CNF SAT_runtime)

add_executable(hyb_sat tests.cpp)
target_link_libraries(hyb_sat SAT_hybrid)
add_test(NAME Hybrid_SAT
        COMMAND $<TARGET_FILE:hyb_sat>)

INSTALL(TARGETS hyb_sat)