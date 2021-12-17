cmake_minimum_required(VERSION 3.10)
project(tplate C)

set(CMAKE_C_STANDARD 99)

if(DEBUG_LOG)
    add_compile_definitions(DEBUG_LOG)
endif()

# For IDE purposes:
set(List_DIR "/var/run/host/usr/local/lib/cmake/List")
set(DString_DIR "/var/run/host/usr/local/lib/cmake/DString")

find_package(List 0.3.1 REQUIRED)
find_package(DString 0.1.0 REQUIRED)


include_directories(include)

add_executable(tplate src/main.c src/project.c src/directory.c src/file.c src/log.c)

target_link_libraries(
    tplate PUBLIC
    List::list
    DString::dstring
)

install(
    TARGETS tplate
    DESTINATION bin
)