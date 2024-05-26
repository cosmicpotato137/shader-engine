cmake_minimum_required(VERSION 3.27)

project(shader-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(CMAKE_CXX_FLAGS 
        "/wd4018 /wd4244 /wd4305 
        /D_CRT_SECURE_NO_WARNINGS 
        /D_CRT_NONSTDC_NO_DEPRECATE 
        /EHsc")
    set(CMAKE_EXE_LINKER_FLAGS "/NODEFAULTLIB:\"MSVCRT\" /NODEFAULTLIB:\"LIBCMT\"")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra") # -Werror
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# window and graphics
find_package(OpenGL REQUIRED)

# boost serialization
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.85.0 COMPONENTS serialization REQUIRED)
link_directories({$ENV{BOOST_ROOT}} ${Boost_LIBRARY_DIRS})

if (WIN32)
find_library(GLFW_LIBRARY NAMES glfw3 PATHS vendor/lib REQUIRED)
find_library(ASSIMP_LIBRARY NAMES assimp-vc143-mt PATHS vendor/lib)

elseif(UNIX AND NOT APPLE)
find_package(glfw3 REQUIRED)
set(GLFW_LIBRARY glfw)
find_package(assimp REQUIRED)
set(ASSIMP_LIBRARY assimp)
endif()

# set core libraries
set(CORE ${GLFW_LIBRARY} ${ASSIMP_LIBRARY} ${Boost_LIBRARIES} ${OPENGL_gl_LIBRARY})

# generic includes for all projects
include_directories(
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/include
    ${Boost_INCLUDE_DIRS}
)

# enable imgui operators
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)

# resource dirs
add_definitions(-DSHADER_ENGINE_BUILD_DIR=\"${CMAKE_SOURCE_DIR}\")

# Include sub-projects.
add_subdirectory("ShaderEngine")
add_subdirectory("ShaderSandbox")

# include external libs
add_subdirectory("vendor")

# Enable testing
enable_testing()

if (WIN32)
add_test(NAME ShaderEngineTest COMMAND ${CMAKE_SOURCE_DIR}/bin/ShaderEngineTest.exe)

elseif(UNIX AND NOT APPLE)
add_test(NAME ShaderEngineTest COMMAND ${CMAKE_SOURCE_DIR}/bin/ShaderEngineTest)
endif()