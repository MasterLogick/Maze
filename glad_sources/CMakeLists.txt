cmake_minimum_required(VERSION 3.2)
project(glad C)

#add library
add_library(glad STATIC include/glad/gl.h include/KHR/khrplatform.h src/gl.c)

#link include headers
target_link_directories(glad PUBLIC include)

#lind dlopen and dlclose functions lib
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})