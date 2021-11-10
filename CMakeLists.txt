cmake_minimum_required(VERSION 3.20)
project(tplate C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

find_package(dbuf REQUIRED)

add_executable(tplate src/main.c src/project.c src/directory.c src/file.c)
