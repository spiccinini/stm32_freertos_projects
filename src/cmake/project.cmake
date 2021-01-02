# Get Build Information
execute_process(COMMAND make --version OUTPUT_VARIABLE MAKE_OUTPUT)
string(REGEX REPLACE "GNU Make ([0-9]\\.[0-9]\\.*[0-9]*).+" "\\1" MAKE_VERSION ${MAKE_OUTPUT})

message("-----------Build Information------------")
message(STATUS "Host    : ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "Make    : ${MAKE_VERSION}")
message(STATUS "CC      : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "CXX     : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build   : ${CMAKE_BUILD_TYPE}")

set(CMAKE_C_FLAGS "${COMPILER_OPTIONS} -lm -lc -lnosys --specs=nosys.specs" )
set(CMAKE_ASM_FLAGS "${COMPILER_OPTIONS}")

# Additional Flags
set(ADDITIONAL_C_FLAGS "-fmessage-length=0 -ffunction-sections -fdata-sections -Wno-error=unused-function -Wall -Wextra -Werror -ggdb -O0")


