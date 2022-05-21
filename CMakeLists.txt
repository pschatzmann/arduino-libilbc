cmake_minimum_required(VERSION 3.16)

# set the project name
project(arduino_libilbc)

# lots of warnings and all warnings as errors
## add_compile_options(-Wall -Wextra )
set(CMAKE_CXX_STANDARD 17)

option(BUILD_MAIN "Build test tool" ON)

file(GLOB_RECURSE SRC_LIST_C CONFIGURE_DEPENDS  "${PROJECT_SOURCE_DIR}/src/*.c" )

# define libraries
add_library (arduino_libilbc ${SRC_LIST_C})

if (BUILD_MAIN)
    add_executable(test ${SRC_LIST_C})
endif()


# prevent compile errors
#target_compile_options(arduino_libilbc PRIVATE  -Wstack-usage=1000 -DILBC_STACK_HACK=1)

# define location for header files
target_include_directories(arduino_libilbc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/src/ilibc_src )
