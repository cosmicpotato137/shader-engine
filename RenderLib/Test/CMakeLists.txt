# GoogleTest is added as a submodule at vendor/googletest
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest ${CMAKE_BINARY_DIR}/vendor/googletest)

file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

# Add executable target
add_executable(RenderLibTest ${SOURCES} ${HEADERS})

# Link GoogleTest and other libraries to your test executable
target_link_libraries(
	RenderLibTest 
	gtest_main
	RenderLib 
	imgui 
	imgui_glfw_opengl3
)

target_include_directories(
	RenderLibTest PRIVATE 
	${PROJECT_SOURCE_DIR}/RenderLib
	${PROJECT_SOURCE_DIR}/vendor/googletest/googletest/include
)