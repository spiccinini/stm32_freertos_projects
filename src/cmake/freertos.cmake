set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/3rd-party/freertos)

# Make sure that git submodule is initialized and updated
if (NOT EXISTS "${FREERTOS_DIR}")
    message(FATAL_ERROR "FreeRTOS not found. ${FREERTOS_DIR}")
endif()

set(FREERTOS_LIB_SRC
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_DIR}/portable/MemMang/heap_4.c
    ${FREERTOS_DIR}/croutine.c
    ${FREERTOS_DIR}/event_groups.c
    ${FREERTOS_DIR}/list.c
    ${FREERTOS_DIR}/queue.c
    ${FREERTOS_DIR}/stream_buffer.c
    ${FREERTOS_DIR}/tasks.c
    ${FREERTOS_DIR}/timers.c
    )

add_library(freertos STATIC ${FREERTOS_LIB_SRC})

target_include_directories(freertos
    PUBLIC 
        ${FREERTOS_DIR}/include
        ${FREERTOS_DIR}/config
        ${FREERTOS_DIR}/portable/GCC/ARM_CM4F
)

# -flto breaks FreeRTOS, so remove it
# string(REPLACE "-flto" "" COMPILER_OPTIMISATION ${COMPILER_OPTIMISATION})
