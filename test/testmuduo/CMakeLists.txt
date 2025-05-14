cmake_minimum_required(VERSION 3.28)
project(main)

#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#设置可执行文件存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置库文件存储路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#配置头文件搜索路径
include_directories(./include)
#配置库文件搜索路径
link_directories({PROJECT_SOURCE_DIR}/lib)

#设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

#把指定路径下所有源文件名放入SRC_LIST中
#aux_source_directory(. SRC_LIST)

#生成可执行文件server
add_executable(server ${SRC_LIST})

#目标程序需要链接的动态库
target_link_libraries(server muduo_net muduo_base pthread)