add_library(CNF)

target_sources(CNF PUBLIC FILE_SET CXX_MODULES FILES CNF.cppm)

add_subdirectory(testgen)