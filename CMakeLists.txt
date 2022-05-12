project(CDCL)
cmake_minimum_required(VERSION 3.14)

set(source ./main.cpp)
include_directories(./include)
# add_compile_options("-O3")
add_executable(main ${source})
add_executable(microsat "microsat.c")