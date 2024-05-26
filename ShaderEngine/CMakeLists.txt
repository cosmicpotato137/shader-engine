# Add your source files here (e.g., .cpp files)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
file(GLOB_RECURSE HEADERS ./src/*.h)

# Choose between platform-specific implementations
if(WIN32)
file(GLOB_RECURSE PLATFORM_SROUCES ./Platform/Windows/*.cpp)
elseif(UNIX AND NOT APPLE)
file(GLOB_RECURSE PLATFORM_SROUCES ./Platform/Linux/*.cpp)
endif()

file(GLOB_RECURSE SHADERS "${CMAKE_SOURCE_DIR}/shaders/*.{frag,vert,compute}")

source_group("Source Files" FILES ${SOURCES} ${PLATFORM_SROUCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_library(ShaderEngine ${SOURCES} ${HEADERS} ${SHADERS} ${PLATFORM_SROUCES})

target_include_directories(ShaderEngine PRIVATE 
  ${CMAKE_SOURCE_DIR}/vendor/include
  ${CMAKE_SOURCE_DIR}/ShaderEngine/src
)
target_link_libraries(ShaderEngine ${CORE} glad imgui imgui_glfw_opengl3)

# precompiled headers
target_precompile_headers(ShaderEngine PRIVATE ./src/pch.h)

# testing
add_subdirectory(Tests)