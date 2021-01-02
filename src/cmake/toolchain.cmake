# Use ARM-GCC for crosscompiling
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)


find_program(ARM_CC arm-none-eabi-gcc)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_OBJDUMP arm-none-eabi-objdump)
find_program(ARM_LD arm-none-eabi-ld)
find_program(ARM_SIZE arm-none-eabi-size)



SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(CMAKE_AR ${ARM_AR})
set(CMAKE_C_COMPILER ${ARM_CC})
set(CMAKE_OBJCOPY ${ARM_OBJCOPY})
set(ARM_OBJDUMP ${ARM_OBJDUMP})
SET(CMAKE_LD ${ARM_LD})
set(CMAKE_SIZE ${ARM_SIZE})

# FOR STM32L4
set(COMPILER_OPTIONS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ${ADDITIONAL_C_FLAGS}")

set(CMAKE_C_FLAGS "${COMPILER_OPTIONS} -lm -lc -lnosys -specs=nosys.specs" )

SET(CMAKE_EXE_LINKER_FLAGS "-nostartfiles -specs=nano.specs -Wl,-Map=linker.map -Wl,-cref -Wl,--gc-sections" CACHE INTERNAL "link flags")


# Dump all the flags at this point
message(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

message("-------------Full Flag List-------------")
message(STATUS "C   : ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_${BUILD_TYPE}}")
message(STATUS "LD  : ${CMAKE_EXE_LINKER_FLAGS}")
