LIST(APPEND SOURCES "main.c")

# file(GLOB_RECURSE MOD_SRC "modules/GUI/*.c")
LIST(APPEND SOURCES "modules/GUI/clock/clock.c")
LIST(APPEND SOURCES "modules/SDCard/sdmmc.c")
LIST(APPEND SOURCES "modules/wifi/wifi_sta.c")
LIST(APPEND SOURCES "modules/mpu6050/mpu.c")
LIST(APPEND SOURCES "modules/ota/ota.c")
LIST(APPEND SOURCES "modules/gpio/input.c")
LIST(APPEND SOURCES "modules/http/myhttp_client.c")

# message(FATAL_ERROR "SOURCES: ${SOURCES}")

LIST(APPEND INCLUDES "modules/GUI/include")
LIST(APPEND INCLUDES "modules/SDCard")
LIST(APPEND INCLUDES "modules/wifi")
LIST(APPEND INCLUDES "modules/mpu6050")
LIST(APPEND INCLUDES "modules/ota")
LIST(APPEND INCLUDES "modules/gpio")
LIST(APPEND INCLUDES "modules/http")

LIST(APPEND INCLUDES "../components")

idf_component_register( SRCS ${SOURCES}
                        INCLUDE_DIRS ${INCLUDES})
