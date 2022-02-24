# 根目录下CMakelists.txt
# 在示例中，使用routes模块中的web作为主入口

# 指定 cmake版本
# 指定 project name
cmake_minimum_required(VERSION 3.10)
project(
    tako
    VERSION 0.1.0
	  LANGUAGES C CXX
)
message("=================================================================")
message("-- [项目名称] project name:  ${CMAKE_PROJECT_NAME}")
message("-- [系统架构] system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message("-- [源目录(项目文件夹)] PROJECT_SOURCE_DIR:  ${PROJECT_SOURCE_DIR} ")
message("-- [二进制目录(build文件夹)] PROJECT_BINARY_DIR:  ${PROJECT_BINARY_DIR} ")

# 设置可执行文件输出目录
set(PROJECT_OUTPUT_DIR ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
message("-- [可执行文件输出目录(build/平台)] output path:  ${PROJECT_OUTPUT_DIR} ")

# 设置可执行文件输出目录后的include文件夹
SET(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)

# 创建文件夹
file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR}) 
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

## 设置目标 运行时/动态/静态链接库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)

## set c++20 编译选项
# -std=c++20 包含C++20特性
set (CMAKE_C_COMPILER "/usr/local/gcc-11.2.0/bin/gcc")
set (CMAKE_CXX_COMPILER "/usr/local/gcc-11.2.0/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")	

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
# aux_source_directory(./src DIR_SRCS)

# INCLUDE for c/C++ include
# include_directories(${PROJECT_INCLUDE_DIR})

# 结尾部分-指定输出文件
#create executable-file
# add_executable(${PROJECT_NAME} src/main.cc )

#包含下级目录src
add_subdirectory(src)
message("=================================================================")


