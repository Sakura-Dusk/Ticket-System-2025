cmake_minimum_required(VERSION 3.22.1)
project(BPT)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(code
        /FileStore/read.hpp
        /FileStore/Chars.hpp
        /FileStore/vector.hpp

        /FileStore/Hash.hpp
        /FileStore/Cache.hpp

        /FileStore/MemoryRiver.hpp
        /FileStore/MemoryRiver_With_Cache.hpp

        /FileStore/FileStore-B+.hpp

        /FileStore/main.cpp
)

# 调试时可以打开，用来测指针的问题
# target_compile_options(code PRIVATE -fsanitize=address,leak,undefined)
# target_link_libraries(code PRIVATE -fsanitize=address,leak,undefined)