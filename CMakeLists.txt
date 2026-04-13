cmake_minimum_required(VERSION 3.10)
project(plugin_echo)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 根据平台设置
if(WIN32)
    message(STATUS "Current platform is Windows")
    set(CORE_LIB_NAME "luo9_core")
    set(CORE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/windows/${CORE_LIB_NAME}.lib")
    set(CORE_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/windows/${CORE_LIB_NAME}.dll")
elseif(UNIX AND NOT APPLE)
    message(STATUS "Current platform is Linux")
    set(CORE_LIB_NAME "luo9_core")
    set(CORE_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/linux/lib${CORE_LIB_NAME}.so")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk/include)
include_directories(${PROJECT_SOURCE_DIR}/sdk)  # 添加sdk目录以找到luo9_sdk.h

# 收集源文件
file(GLOB SRC_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB SDK_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/sdk/*.cpp")

# 创建共享库
add_library(plugin_echo SHARED ${SRC_LIST} ${SDK_LIST})

# 检查库文件是否存在
if(NOT EXISTS ${CORE_LIB_PATH})
    message(WARNING "Core library not found at: ${CORE_LIB_PATH}")
endif()

# 链接核心库
target_link_libraries(plugin_echo ${CORE_LIB_PATH})

# 设置输出目录
set_target_properties(plugin_echo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Windows下处理DLL复制
if(WIN32)
    # 确保DLL在运行时可以被找到
    add_custom_command(TARGET plugin_echo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CORE_DLL_PATH}"
            $<TARGET_FILE_DIR:plugin_echo>)
    
    # 如果需要复制到输出目录
    add_custom_command(TARGET plugin_echo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CORE_DLL_PATH}"
            "${CMAKE_BINARY_DIR}/bin/")
endif()

# Linux下设置RPATH
if(UNIX AND NOT APPLE)
    set_target_properties(plugin_echo PROPERTIES
        INSTALL_RPATH "\$ORIGIN/lib")
endif()

# 打印链接信息（用于调试）
message(STATUS "Core library path: ${CORE_LIB_PATH}")
message(STATUS "Plugin output dir: ${CMAKE_BINARY_DIR}/bin")