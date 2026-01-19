cmake_minimum_required(VERSION 3.10)
project(OJP VERSION 1.0)

#检查配置编译器是否为 GNU C++
if(CMAKE_COMPILER_IS_GNUCXX)
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动搜寻头文件路径
include_directories(include)
# 查找安装在系统中的第三方依赖
find_package(Threads REQUIRED)

# 找到所有的源文件（递归搜索 src 目录下所有 .cpp）
file(GLOB_RECURSE COMPILESOURCES "src/compile/*.cpp")
file(GLOB_RECURSE WEBSERVERSOURCES "src/webServer/*.cpp")

# 生成可执行程序
add_executable(compileServer ${COMPILESOURCES})
add_executable(webServer ${WEBSERVERSOURCES})

# 连接线程库
target_link_libraries(compileServer 
    PRIVATE Threads::Threads)
target_link_libraries(webServer 
    PRIVATE Threads::Threads)