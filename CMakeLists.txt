cmake_minimum_required(VERSION 3.26)
include(cmake/vcpkg.cmake)

vcpkg_install_if_not_found(glfw3 boost-asio boost-beast freetype)
set(CMAKE_CXX_STANDARD 26)
project(nullcore_loader)
add_executable(nullcore_loader source/main.cpp)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(nullcore_loader PRIVATE glfw cv-sdk imgui opengl32)


add_subdirectory(extlibs)
add_subdirectory(source)