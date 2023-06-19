cmake_minimum_required(VERSION 2.5)
project(m_muduo)

# 设置调试信息,启动C++11语言标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")


#设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# m_muduo最终编译成so动态库，设置动态库的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


# # 定义参与编译的源文件
# aux_source_directory(. SRC_LIST)


#设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/include)
#src包含了mprpc框架的所有源代码
add_subdirectory(src)
