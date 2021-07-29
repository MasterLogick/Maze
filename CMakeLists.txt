cmake_minimum_required(VERSION 3.15)
project(Maze VERSION 0.0.1 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

#generate shader header file
file(GLOB Shaders_dir res/shaders/*)
file(READ cmake/ShaderLoader.in Shader.in)
set(Definitions "")
set(Declarations "")
set(C_code "")
foreach (Shader_dir ${Shaders_dir})
    cmake_path(GET Shader_dir FILENAME Program_name)
    set(Program_var_name "${Program_name}_Shader")
    string(APPEND Definitions "extern ShaderProgram *${Program_var_name};\n")
    string(APPEND Declarations "ShaderProgram *${Program_var_name} = nullptr;\n")
    string(APPEND C_code "${Program_var_name} = new ShaderProgram();\n")
    file(GLOB Shaders ${Shader_dir}/*)
    set(Shaders_C_codes "")
    foreach (Shader ${Shaders})
        cmake_path(GET Shader STEM Shader_type)
        string(TOUPPER ${Shader_type} Shader_type)
        string(APPEND Definitions "extern Shader *${Program_name}_Shader_${Shader_type};\n")
        string(APPEND Declarations "Shader *${Program_name}_Shader_${Shader_type} = nullptr;\n")
        file(READ ${Shader} Shader_code)
        configure_file(cmake/ShaderLoader.in cmake/ShaderLoader @ONLY)
        file(READ ${CMAKE_CURRENT_BINARY_DIR}/cmake/ShaderLoader Shader_C_code)
        string(APPEND Shaders_C_codes "${Shader_C_code}")
    endforeach ()
    string(APPEND C_code "${Shaders_C_codes}${Program_var_name}->link();\n")
endforeach ()
configure_file(${PROJECT_SOURCE_DIR}/src/graphics/shader/Shaders.h.in ${PROJECT_SOURCE_DIR}/src/graphics/shader/Shaders.h @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/graphics/shader/Shaders.cpp.in ${PROJECT_SOURCE_DIR}/src/graphics/shader/Shaders.cpp @ONLY)

#load glad lib project
add_subdirectory(glad)

#load glfw lib project
set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

#load glm lib project
#set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
add_subdirectory(glm)

#adding main sources
file(GLOB_RECURSE SOURCE_CPP_FILES "src/*.cpp")
file(GLOB_RECURSE SOURCE_H_FILES "src/*.h")
add_executable(Maze ${SOURCE_CPP_FILES} ${SOURCE_H_FILES})

#set glfw not to include glad
target_compile_definitions(Maze PUBLIC GLFW_INCLUDE_NONE)

#link libraries
target_link_libraries(Maze PUBLIC glfw glad glm)