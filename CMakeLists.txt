cmake_minimum_required(VERSION 3.10)
project(tplate C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(foreign/dbuf)

include_directories(include)

add_executable(tplate src/main.c src/project.c src/directory.c src/file.c src/dstring.c)

target_link_libraries(tplate PUBLIC dbuf)