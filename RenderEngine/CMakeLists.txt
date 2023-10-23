# Add your source files here (e.g., .cpp files)
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_executable(RenderEngine ${SOURCES} ${HEADERS} ${SHADERS})
# steps necessary to link an external library!
target_link_libraries(RenderEngine RenderLib)
target_include_directories(RenderEngine PUBLIC 
	${PROJECT_SOURCE_DIR}/vendor/include
	${PROJECT_SOURCE_DIR}/RenderLib)

add_custom_command(
    TARGET RenderEngine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/vendor/lib/assimp-vc143-mt.dll
        $<TARGET_FILE_DIR:RenderEngine>
)