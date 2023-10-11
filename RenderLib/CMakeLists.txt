# Add your source files here (e.g., .cpp files)
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)
file(GLOB_RECURSE SHADERS ../shaders/*.vert ../shaders/*.frag)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
source_group("Shader Files" FILES ${SHADERS})

# Add executable target
add_library(RenderLib ${SOURCES} ${HEADERS})

target_include_directories(RenderLib PRIVATE vendor/include)
target_link_directories(RenderLib PUBLIC vendor/lib)
target_link_libraries(RenderLib ${CORE})