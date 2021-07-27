cmake_minimum_required(VERSION 3.2)
project(glad C)

#add library
add_library(glad include/glad/gl.h include/KHR/khrplatform.h src/gl.c)

#link include headers
target_include_directories(glad PUBLIC include)

#lind dlopen and dlclose functions lib
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})