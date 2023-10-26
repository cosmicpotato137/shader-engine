# Add your source files here (e.g., .cpp files)
file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag ../shaders/*.shader ../shaders/*.compute)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_library(RenderLib ${SOURCES} ${HEADERS} ${SHADERS})

target_include_directories(RenderLib PRIVATE ${CMAKE_SOURCE_DIR}/vendor/include)
target_link_directories(RenderLib PUBLIC ${CMAKE_SOURCE_DIR}/vendor/lib)
target_link_libraries(RenderLib ${CORE})
target_link_libraries(RenderLib GLEW::GLEW)