cmake_minimum_required(VERSION 3.10)

project(shader-ideas)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS 
    "/wd4018 /wd4244 /wd4305 
    /D_CRT_SECURE_NO_WARNINGS 
    /D_CRT_NONSTDC_NO_DEPRECATE 
    /EHsc")

set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT\" /NODEFAULTLIB:\"LIBCMT\"")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# set up packages and libraries
find_package(OpenGL REQUIRED)
find_library(GLFW NAMES glfw3 PATHS vendor/lib)
find_library(ASSIMP NAMES assimp-vc143-mt PATHS vendor/lib)
set(CORE glad ${GLFW} ${ASSIMP} opengl32.lib)

# generic includes for all projects
include_directories(${CMAKE_SOURCE_DIR}/vendor
                    ${CMAKE_SOURCE_DIR}/vendor/include)

link_directories(${CMAKE_SOURCE_DIR}/vendor/include/ImGui-FileDialog)

# resource dirs
add_definitions(-DSHADER_DIR=\"${CMAKE_SOURCE_DIR}/shaders\")
add_definitions(-DRESOURCE_DIR=\"${CMAKE_SOURCE_DIR}/res\")
add_definitions(-DTEXTURE_DIR=\"${CMAKE_SOURCE_DIR}/res/textures\")

# enable imgui operators
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# Include sub-projects.
add_subdirectory("RenderLib")
add_subdirectory("RenderEngine")

# include external libs
add_subdirectory("vendor")

# Enable testing
enable_testing()

add_test(NAME RenderLibTest COMMAND ${CMAKE_SOURCE_DIR}/bin/RenderLibTest.exe)