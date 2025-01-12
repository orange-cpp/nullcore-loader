//
// Created by Orange on 1/12/2025.
//
#include <cstdio>
#include <CodeVirtualizer/VirtualizerSDK.h>
#include <CodeVirtualizer/StealthCodeArea.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "gui/StatusWindow.hpp"

void CvStealthArea()
{
    const auto CVSX = [](unsigned int) {};
    STEALTH_AREA_START

    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK
    STEALTH_AREA_CHUNK

    STEALTH_AREA_END
}

int main()
{
    VIRTUALIZER_FALCON_TINY_START

    if (!glfwInit())
        std::exit(EXIT_FAILURE);

    ncc_loader::gui::StatusWindow status_window;

    if (reinterpret_cast<uintptr_t>(&main) == 0x1111)
        CvStealthArea();

    VIRTUALIZER_FALCON_TINY_END
    return EXIT_SUCCESS;
}