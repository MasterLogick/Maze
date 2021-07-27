cmake_minimum_required(VERSION 3.15)
project(Maze VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

#load glad lib project
add_subdirectory(glad/cmake)
glad_add_library(glad STATIC LOADER API gl:core=4.6)

#load glfw lib project
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

#adding main sources
file(GLOB_RECURSE SOURCE_FILES "src/*")
add_executable(Maze ${SOURCE_FILES})

#set glfw not to include glad
target_compile_definitions(Maze PUBLIC GLFW_INCLUDE_NONE)

#add headers of libs
target_include_directories(Maze PUBLIC glfw/include)

#link libraries
target_link_libraries(Maze PUBLIC glfw glad)