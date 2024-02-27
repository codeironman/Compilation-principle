cmake_minimum_required(VERSION 3.26)
project()

set(CMAKE_CXX_STANDARD 17)

add_executable(main.cpp)

add_executable(test)
add_executable(NFA NFA.cpp)
add_executable(main main.cpp)
add_executable(NFA_test NFA_test.cpp)