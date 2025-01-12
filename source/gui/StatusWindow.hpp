//
// Created by Orange on 1/12/2025.
//
#pragma once

struct GLFWwindow;


namespace ncc_loader::gui
{
    class StatusWindow
    {
    public:
        StatusWindow();

    private:
        GLFWwindow* m_window;
    };
}
