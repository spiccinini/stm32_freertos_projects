cmake_minimum_required(VERSION 3.9)
project(${APP_NAME} C)

set(DEVICE STM32L433)
set(PLATFORM_ARCH STM32L4)
add_definitions(-D${PLATFORM_ARCH})

include(cmake/project.cmake)
include(cmake/toolchain.cmake)
include(cmake/freertos.cmake)
include(cmake/libopencm3.cmake)

include(apps/${APP_NAME}/app_config.cmake)

message(STATUS "SOURCE FILES : ${APP_SRC_LIST}")

add_executable(${PROJECT_NAME}.elf ${APP_SRC_LIST})

target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32l4 freertos)
target_link_options(${PROJECT_NAME}.elf BEFORE
                    PRIVATE -T${CMAKE_SOURCE_DIR}/link_scripts/STM32L433RCT6.ld ${ADDITIONAL_LINKER_FLAGS})

target_include_directories(${PROJECT_NAME}.elf
    PRIVATE
        ${LIBOPENCM3_DIR}/include
)


add_custom_target(${PROJECT_NAME}.hex ALL
    DEPENDS ${PROJECT_NAME}.elf 
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating ${PROJECT_NAME}.hex"
)

add_custom_target(${PROJECT_NAME}.bin ALL
    DEPENDS ${PROJECT_NAME}.elf 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating ${PROJECT_NAME}.bin"
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley "${PROJECT_NAME}.elf")
