# Add your source files here (e.g., .cpp files)
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag ../shaders/*.shader ../shaders/*.compute)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_library(RenderEngine ${SOURCES} ${HEADERS} ${SHADERS})

# steps necessary to link an external library!
target_link_libraries(RenderEngine ShaderEngine imgui imgui_glfw_opengl3)
target_include_directories(RenderEngine PRIVATE ${PROJECT_SOURCE_DIR}/ShaderEngine)

# testing
add_subdirectory(Test)