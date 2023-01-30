# 设置 CMake 的最低版本
cmake_minimum_required(VERSION 3.20)

# 选择工具链
set(CMAKE_TOOLCHAIN_FILE mingw.cmake)

# 设置项目名称
project(Main VERSION 1.0)

# 扫描当前目录下源文件
aux_source_directory(. PROJECT_SRC_LIST)

# 添加可执行目标源文件
add_executable(${PROJECT_NAME} ${PROJECT_SRC_LIST})

# 查看可执行文件大小
add_custom_command(
    TARGET  ${PROJECT_NAME}
    POST_BUILD COMMAND ${CMAKE_SIZE}  
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe)
