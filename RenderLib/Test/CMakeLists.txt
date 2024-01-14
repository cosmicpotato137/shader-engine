# Include FetchContent module
include(FetchContent)

# Declare GoogleTest as a content to fetch
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/heads/master.zip
	DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/vendor/googletest
	BINARY_DIR ${CMAKE_SOURCE_DIR}/vendor/googletest
)

# Fetch GoogleTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
	${googletest_SOURCE_DIR}/googletest/include
)