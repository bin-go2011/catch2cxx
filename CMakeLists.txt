cmake_minimum_required(VERSION 3.13)
project(Catch2Cxx)

add_library(Catch2::Catch2 INTERFACE IMPORTED)
set_target_properties(Catch2::Catch2 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/catch2"
)

add_executable(foo test.cpp)
target_link_libraries(foo Catch2::Catch2)