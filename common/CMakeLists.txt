add_library(CNF)

target_sources(CNF PUBLIC FILE_SET CXX_MODULES FILES CNF.cppm)

if (ENABLE_TESTGEN)
    add_subdirectory(testgen)
endif()