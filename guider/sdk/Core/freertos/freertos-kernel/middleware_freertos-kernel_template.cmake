# Add set(CONFIG_USE_middleware_freertos-kernel_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_CORE STREQUAL cm33 AND (CONFIG_FPU STREQUAL NO_FPU OR CONFIG_FPU STREQUAL SP_FPU) AND (CONFIG_DEVICE_ID STREQUAL MCXN546 OR CONFIG_DEVICE_ID STREQUAL MCXN946 OR CONFIG_DEVICE_ID STREQUAL MCXN947))
add_config_file(${CMAKE_CURRENT_LIST_DIR}/../../source/FreeRTOSConfig.h ${CMAKE_CURRENT_LIST_DIR}/template/ARM_CM33_3_priority_bits middleware_freertos-kernel_template)
endif()

