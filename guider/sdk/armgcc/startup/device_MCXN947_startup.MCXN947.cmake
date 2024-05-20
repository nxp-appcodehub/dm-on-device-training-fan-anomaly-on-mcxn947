# Add set(CONFIG_USE_device_MCXN947_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MCXN947_system)

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/startup_MCXN947_cm33_core0.S
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core1)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
#      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MCXN947_cm33_core1.S
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
#      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mcxn947_cm33_core0.c
#      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mcxn947_cm33_core0.cpp
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core1)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
#      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mcxn947_cm33_core1.c
#      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mcxn947_cm33_core1.cpp
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
#      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/boot_multicore_slave.c
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MCXN947_cm33_core0.S "" device_MCXN947_startup.MCXN947)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core1)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MCXN947_cm33_core1.S "" device_MCXN947_startup.MCXN947)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core0)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MCXN947_cm33_core0.s "" device_MCXN947_startup.MCXN947)
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33 AND CONFIG_CORE_ID STREQUAL cm33_core1)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MCXN947_cm33_core1.s "" device_MCXN947_startup.MCXN947)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
)
endif()

else()

message(SEND_ERROR "device_MCXN947_startup.MCXN947 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
