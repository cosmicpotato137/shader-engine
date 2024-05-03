file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

# Add executable target
add_executable(RenderEngineTest ${SOURCES} ${HEADERS})

# Link GoogleTest and other libraries to your test executable
target_link_libraries(
	RenderEngineTest PRIVATE
  ShaderEngine
	RenderEngine
	gtest_main
	imgui 
	imgui_glfw_opengl3
)

target_include_directories(
	RenderEngineTest PRIVATE 
	${PROJECT_SOURCE_DIR}/ShaderEngine
	${PROJECT_SOURCE_DIR}/RenderEngine
	${PROJECT_SOURCE_DIR}/vendor/googletest/googletest/include
)