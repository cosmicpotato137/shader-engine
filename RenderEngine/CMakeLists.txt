# Add your source files here (e.g., .cpp files)
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_executable(RenderEngine ${SOURCES} ${HEADERS})
 
# steps necessary to link an external library!
target_link_libraries(RenderEngine CPSL)
target_include_directories(RenderEngine PUBLIC
                           "${PROJECT_SOURCE_DIR}"/RenderLib
                           )