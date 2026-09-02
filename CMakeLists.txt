cmake_minimum_required(VERSION 3.14)
project(cpp-thread-pool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译警告配置
if (MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -pedantic -pthread)
endif()

include_directories(include)

# 演示可执行程序
add_executable(thread_pool_demo src/main.cpp)
target_link_libraries(thread_pool_demo PRIVATE pthread)

# 单元测试可执行程序
add_executable(test_thread_pool tests/test_thread_pool.cpp)
target_link_libraries(test_thread_pool PRIVATE pthread)
