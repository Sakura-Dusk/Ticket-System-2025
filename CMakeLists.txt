cmake_minimum_required(VERSION 3.22.1)
project(BPT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(code
        /FileStore/main.cpp
)

# 调试时可以打开，用来测指针的问题
# target_compile_options(code PRIVATE -fsanitize=address,leak,undefined)
# target_link_libraries(code PRIVATE -fsanitize=address,leak,undefined)