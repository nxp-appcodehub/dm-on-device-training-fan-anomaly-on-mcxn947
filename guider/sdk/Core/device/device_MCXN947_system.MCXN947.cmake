# Add set(CONFIG_USE_device_MCXN947_system true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MCXN947_CMSIS)

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/system_MCXN947_cm33_core0.c
)
endif()

if(CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core1)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
#      ${CMAKE_CURRENT_LIST_DIR}/system_MCXN947_cm33_core1.c
)
endif()

if(CONFIG_CORE STREQUAL cm33)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
${CMAKE_CURRENT_LIST_DIR}/
)
endif()

else()

message(SEND_ERROR "device_MCXN947_system.MCXN947 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
