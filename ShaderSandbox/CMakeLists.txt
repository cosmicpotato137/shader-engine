# Add your source files here (e.g., .cpp files)
file(GLOB_RECURSE SOURCES ./src/*.cpp)
file(GLOB_RECURSE HEADERS ./src/*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag ../shaders/*.shader ../shaders/*.compute)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

add_executable(ShaderSandbox WIN32 
    ${SOURCES} ${HEADERS} ${SHADERS}
    ${CMAKE_SOURCE_DIR}/res/resource.rc)

# steps necessary to link an external library!
target_link_libraries(ShaderSandbox ShaderEngine imgui imgui_glfw_opengl3)
target_include_directories(ShaderSandbox PRIVATE
    ${PROJECT_SOURCE_DIR}/ShaderEngine/src
    ${PROJECT_SOURCE_DIR}/ShaderSandbox/src
)

add_custom_command(
    TARGET ShaderSandbox 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/vendor/lib/assimp-vc143-mt.dll
        $<TARGET_FILE_DIR:ShaderSandbox>
)