//
// Created by Orange on 1/12/2025.
//
#pragma once
#include <Windows.h>
#include <GL/gl.h>
#include <imgui.h>

struct GLFWwindow;
#include <boost/asio.hpp>


namespace ncc_loader::gui
{
    class StatusWindow
    {
    public:
        StatusWindow();
        void RunOnce();
        void Task();

        std::string m_text;
        GLFWwindow* m_window;

        boost::asio::io_service m_io_service;
        boost::asio::io_service::work m_work;

        boost::asio::steady_timer m_timer;
        boost::asio::steady_timer m_renderTimer;
        ImGuiContext* m_context;
        GLuint m_logoTexture{};
    };
}
