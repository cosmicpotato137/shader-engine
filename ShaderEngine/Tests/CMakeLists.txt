# Fetch testing environment
# Include FetchContent module
include(FetchContent) 

# Declare GoogleTest as a content to fetch
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/heads/master.zip
	DOWNLOAD_DIR ${CMAKE_SOURCE_DIR}/vendor/googletest
	BINARY_DIR ${CMAKE_SOURCE_DIR}/vendor/googletest
    DOWNLOAD_EXTRACT_TIMESTAMP true
)

# Fetch GoogleTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB SOURCES ./*.cpp)
file(GLOB HEADERS ./*.h)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

# Add executable target
add_executable(ShaderEngineTest ${SOURCES} ${HEADERS})

# Link GoogleTest and other libraries to your test executable
target_link_libraries(
	ShaderEngineTest 
	gtest
	gtest_main
	ShaderEngine 
	imgui 
	imgui_glfw_opengl3
)

target_include_directories(
	ShaderEngineTest PRIVATE 
	${PROJECT_SOURCE_DIR}/ShaderEngine/src
)