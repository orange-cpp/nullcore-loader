add_subdirectory(code-virtualizer-sdk)


project(imgui)

add_library(imgui STATIC imgui/imgui.cpp)
target_include_directories(imgui PUBLIC imgui imgui/backends imgui/misc/freetype)
target_link_libraries(imgui PUBLIC glfw Freetype::Freetype)
target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE)
target_sources(imgui PRIVATE imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/misc/freetype/imgui_freetype.cpp)

project(stb)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

project(xorstr)
add_library(xorstr INTERFACE)
target_include_directories(xorstr INTERFACE xorstr/include)

add_subdirectory(traypp)