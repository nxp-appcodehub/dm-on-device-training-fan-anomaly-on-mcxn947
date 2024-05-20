# Add set(CONFIG_USE_driver_ssd1963 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_dbi)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/fsl_ssd1963.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
${CMAKE_CURRENT_LIST_DIR}/
)

else()

message(SEND_ERROR "driver_ssd1963.MCXN947 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
