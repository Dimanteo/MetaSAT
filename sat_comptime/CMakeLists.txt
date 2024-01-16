# Tests for library
add_executable(ct_sat ct_sat.cpp)
target_link_libraries(ct_sat CNF)
add_test(NAME Compiletime_SAT
        COMMAND $<TARGET_FILE:ct_sat>)

INSTALL(TARGETS ct_sat)