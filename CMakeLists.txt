cmake_minimum_required(VERSION 3.22)
project(coroutines)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-fcoroutines")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(basic_example basics/main.cpp)
add_executable(fibo_generator generator/fibonacci/main.cpp)
add_executable(lazy_factorial lazy/factorial/main.cpp)
