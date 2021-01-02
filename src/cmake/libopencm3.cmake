set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/3rd-party/libopencm3)

# Make sure that git submodule is initialized and updated
if (NOT EXISTS ${LIBOPENCM3_DIR}/Makefile)
    message(FATAL_ERROR "libopencm3 submodule not found. Initialize with 'git submodule update --init' in the source directory")
endif()

SET(LIBOPENCM3_FILE ${LIBOPENCM3_DIR}/lib/libopencm3_stm32l4.a)

add_custom_target(libopencm3)

add_custom_command(TARGET libopencm3
                   PRE_BUILD
                   COMMAND make -C ${LIBOPENCM3_DIR} TARGETS=stm32/l4
                   COMMAND ${CMAKE_COMMAND} -E copy ${LIBOPENCM3_FILE} ${CMAKE_BINARY_DIR}
                   )

add_library(opencm3_stm32l4 STATIC IMPORTED GLOBAL)
add_dependencies(opencm3_stm32l4 libopencm3)

# specify where the library is and where to find the headers
set_target_properties(opencm3_stm32l4
    PROPERTIES
    IMPORTED_LOCATION ${LIBOPENCM3_FILE}
    INTERFACE_INCLUDE_DIRECTORIES /tmp)
