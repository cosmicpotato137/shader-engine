cmake_minimum_required(VERSION 3.10)

project(shader-ideas)

set(CMAKE_CXX_STANDARD 17)
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

include_directories(${CMAKE_SOURCE_DIR}/vendor
                    ${CMAKE_SOURCE_DIR}/vendor/include)

# Include sub-projects.
add_subdirectory ("RenderLib")
add_subdirectory ("RenderEngine")

# include external libs
add_subdirectory("vendor")
