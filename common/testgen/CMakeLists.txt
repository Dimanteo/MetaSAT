set(GENERATED_FILE ${CMAKE_BINARY_DIR}/autogen_tests.h)
set(TESTGEN ${CMAKE_CURRENT_SOURCE_DIR}/testgen.py)

add_custom_command(OUTPUT ${GENERATED_FILE}
    COMMAND python3 ${TESTGEN} ${GENERATED_FILE}
    DEPENDS ${TESTGEN}
)

add_custom_target(generate_tests DEPENDS ${GENERATED_FILE})