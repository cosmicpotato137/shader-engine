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