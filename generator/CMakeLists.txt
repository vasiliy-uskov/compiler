cmake_minimum_required(VERSION 3.14)
project(Generator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(programmObjectModel)
add_subdirectory(tests)

add_library(Generator STATIC Generator.cpp)

target_link_libraries(
  Generator
  ProgrammObjectModel
)