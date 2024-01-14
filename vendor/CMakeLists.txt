# glad
add_library(glad glad/src/gl.c)
target_include_directories(glad SYSTEM PUBLIC glad/include)

# imgui
add_library(imgui imgui/imgui.cpp imgui/imgui_widgets.cpp imgui/imgui_tables.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp)
target_link_directories(RenderLib PUBLIC imgui)
target_include_directories(imgui SYSTEM PUBLIC imgui)

# imgui backend
add_library(imgui_glfw_opengl3 imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_compile_definitions(imgui_glfw_opengl3 PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui_glfw_opengl3 imgui)
target_link_libraries(imgui_glfw_opengl3 glad)
target_link_libraries(imgui_glfw_opengl3 ${GLFW})
target_include_directories(imgui_glfw_opengl3 SYSTEM PUBLIC imgui/backends)
