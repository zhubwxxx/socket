cmake_minimum_required(VERSION 3.10)
project(test CXX)

# 声明库类型，如果此变量不设置，构建为静态库
set(BUILD_SHARED_LIBS SHARED)

# 添加头文件搜索路径 CXX_INCLUDES
# include_directories(${PROJECT_SOURCE_DIR})
# 添加编译选项 CXX_FLAGS 
add_compile_options(-std=c++11 -fstack-protector-all)

# 设置安装目录
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
# 设置安装目录后，将test安装到${CMAKE_SOURCE_DIR}/bin目录下
# install(TARGETS test DESTINATION bin)

add_subdirectory(src)


