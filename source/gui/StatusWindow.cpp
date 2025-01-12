//
// Created by Orange on 1/12/2025.
//
#define GLFW_EXPOSE_NATIVE_WIN32
#include "StatusWindow.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


namespace ncc_loader::gui
{
    StatusWindow::StatusWindow()
    {
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_DECORATED, false);
        glfwWindowHint(GLFW_FLOATING, true);
        m_window = glfwCreateWindow(512, 192, "status window", nullptr, nullptr);

        SetWindowLong(glfwGetWin32Window(m_window), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
        glfwSetWindowAttrib(m_window, GLFW_MOUSE_PASSTHROUGH, true);

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_window, (mode->width - 512) / 2, (mode->height - 192) / 2);

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 460");

        glfwShowWindow(m_window);
        while (!glfwWindowShouldClose(m_window))
        {
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::GetForegroundDrawList()->AddText({}, ImColor(255,0,255), "Hello World!");
            ImGui::Render();


            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_window);
        }
    }
}
