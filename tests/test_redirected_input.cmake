if(NOT DEFINED EFOR_EXECUTABLE OR NOT DEFINED TEST_BINARY_DIR)
    message(FATAL_ERROR "EFOR_EXECUTABLE and TEST_BINARY_DIR must be provided")
endif()

set(empty_input "${TEST_BINARY_DIR}/efor_redirected_stdin.tmp")
file(WRITE "${empty_input}" "")

execute_process(
    COMMAND "${EFOR_EXECUTABLE}"
    INPUT_FILE "${empty_input}"
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr
    RESULT_VARIABLE exit_code
    TIMEOUT 5
)

file(REMOVE "${empty_input}")

if("${exit_code}" STREQUAL "0")
    message(FATAL_ERROR "EFOR unexpectedly succeeded with redirected stdin")
endif()

if(NOT stderr MATCHES "Failed to initialize input platform")
    message(FATAL_ERROR "Expected input initialization error; stderr was: ${stderr}")
endif()
