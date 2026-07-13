cmake_minimum_required(VERSION 3.10)
project(plugin_echo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDK paths
set(SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdk")
set(SDK_INCLUDE "${SDK_DIR}/include")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${SDK_INCLUDE})

# Collect source files
file(GLOB SRC_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create shared library
add_library(plugin_echo SHARED ${SRC_LIST})

# Platform and compiler specific linking
if(WIN32)
    message(STATUS "Current platform is Windows")
    if(MSVC)
        message(STATUS "Using MSVC compiler")
        target_link_libraries(plugin_echo
            "${SDK_DIR}/lib/windows/luo9_sdk_cpp.lib"
            "${SDK_DIR}/lib/windows/luo9_core.lib"
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        message(STATUS "Using MinGW GCC compiler")
        target_link_libraries(plugin_echo
            "${SDK_DIR}/lib/windows/libluo9_sdk_cpp_mingw.a"
            "${SDK_DIR}/lib/windows/luo9_core.lib"
        )
        target_link_options(plugin_echo PRIVATE -static-libgcc -static-libstdc++)
    endif()
    # Copy DLL to output
    add_custom_command(TARGET plugin_echo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDK_DIR}/lib/windows/luo9_core.dll"
            $<TARGET_FILE_DIR:plugin_echo>)
elseif(UNIX AND NOT APPLE)
    message(STATUS "Current platform is Linux")
    target_link_libraries(plugin_echo
        "${SDK_DIR}/lib/linux/libluo9_sdk_cpp.a"
        "${SDK_DIR}/lib/linux/libluo9_core.so"
        pthread
    )
    set_target_properties(plugin_echo PROPERTIES
        BUILD_RPATH "${SDK_DIR}/lib/linux"
        INSTALL_RPATH "\$ORIGIN/../lib")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Output directory
set_target_properties(plugin_echo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

message(STATUS "SDK directory: ${SDK_DIR}")
message(STATUS "Plugin output dir: ${CMAKE_BINARY_DIR}/bin")
