cmake_minimum_required(VERSION 3.27)

project(shader-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

# window and graphics
find_package(OpenGL REQUIRED)
find_library(GLFW NAMES glfw3 PATHS vendor/lib REQUIRED)

# boost serialization
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.85.0 COMPONENTS serialization REQUIRED)
link_directories($ENV{BOOST_ROOT}/stage/lib ${Boost_LIBRARY_DIRS})

# asset importing
find_library(ASSIMP NAMES assimp-vc143-mt PATHS vendor/lib)

# set core libraries
set(CORE ${GLFW} ${ASSIMP} ${Boost} opengl32.lib)

# generic includes for all projects
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/include
    ${Boost_INCLUDE_DIRS}
)

# enable imgui operators
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# Set macro definition for root directory
add_definitions(-DROOT_DIR=\"${CMAKE_SOURCE_DIR}\")

# resource dirs
add_definitions(-DRESOURCE_DIR=\"${CMAKE_SOURCE_DIR}/res\")
add_definitions(-DSHADER_DIR=\"${CMAKE_SOURCE_DIR}/res/shaders\")
add_definitions(-DTEXTURE_DIR=\"${CMAKE_SOURCE_DIR}/res/textures\")

# Include sub-projects.
add_subdirectory("ShaderEngine")
add_subdirectory("ShaderSandbox")

# include external libs
add_subdirectory("vendor")

# Enable testing
enable_testing()

add_test(NAME ShaderEngineTest COMMAND ${CMAKE_SOURCE_DIR}/bin/ShaderEngineTest.exe)