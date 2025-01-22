//
// Created by Orange on 1/12/2025.
//
#define GLFW_EXPOSE_NATIVE_WIN32
#include "StatusWindow.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <imgui.h>
#include <imgui_freetype.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <CodeVirtualizer/VirtualizerSDK.h>
#include <array>
#include <chrono>
#include <format>
#include "images/ncc_loader_logo.h"
#include <xorstr.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


std::vector<std::string> SplitLines(const std::string& text)
{
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line, '\n'))
    {
        lines.push_back(line);
    }
    return lines;
}

bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load_from_memory((const unsigned char*)data, (int)data_size, &image_width, &image_height, nullptr, 4);
    if (image_data == nullptr)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;

    if (out_width)
        *out_width = image_width;
    if (out_height)
        *out_height = image_height;


    return true;
}

void DrawCenteredTextureForeground(
    ImTextureID textureId,
    const ImVec2& textureSize,
    float posY,
    ImU32 color = IM_COL32_WHITE,
    const ImVec2& uvMin = ImVec2(0.0f, 0.0f),
    const ImVec2& uvMax = ImVec2(1.0f, 1.0f))
{
    // Get the size of the current display (window).
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    // Access the foreground draw list.
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // Calculate the X coordinate so the texture is centered.
    float startX = (windowSize.x - textureSize.x) * 0.5f;

    // Compute the bottom-right corner of the texture.
    ImVec2 topLeft(startX, posY);
    ImVec2 bottomRight(startX + textureSize.x, posY + textureSize.y);

    // Draw the texture using the foreground draw list.
    drawList->AddImage(textureId, topLeft, bottomRight, uvMin, uvMax, color);
}

// Function to draw text with \n-separated lines centered using the foreground draw list
void DrawCenteredTextForeground(const std::string& text, ImU32 color = IM_COL32(255, 255, 255, 255))
{
    // Split the input text into lines
    std::vector<std::string> lines = SplitLines(text);

    // Get the size of the ImGui window
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    // Access the foreground draw list
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // Calculate the vertical starting point to center the text block
    float totalTextHeight = lines.size() * ImGui::GetTextLineHeightWithSpacing();
    float startY = (windowSize.y - totalTextHeight) * 0.5f;

    // Iterate over the lines of text and render them
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const std::string& line = lines[i];

        // Calculate the width of the text line
        float textWidth = ImGui::CalcTextSize(line.c_str()).x;

        // Calculate the position to center the text
        float startX = (windowSize.x - textWidth) * 0.5f;
        float posY = startY + i * ImGui::GetTextLineHeightWithSpacing();

        // Draw the text using the foreground draw list
        drawList->AddText(ImVec2(startX, posY), color, line.c_str());
    }
}


namespace ncc_loader::gui
{
    void StatusWindow::RunOnce()
    {
        VIRTUALIZER_MUTATE_ONLY_START
        auto start = std::chrono::high_resolution_clock::now();
        glfwMakeContextCurrent(m_window);
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        DrawCenteredTextForeground(m_text, ImColor(255, 255, 255));
        DrawCenteredTextureForeground(m_logoTexture, {64, 64}, 8);
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);

        const auto end = std::chrono::high_resolution_clock::now();
        m_renderTimer.expires_after(std::chrono::milliseconds(1000/30) - std::chrono::duration_cast<std::chrono::milliseconds>(end-start));
        m_renderTimer.async_wait([this](const auto&)
        {
            RunOnce();
        });
        VIRTUALIZER_MUTATE_ONLY_END
    }

    void StatusWindow::Task()
    {
        static int dotsCount = 0;

        if (dotsCount++ >= 3)
            dotsCount = 0;

        m_text = std::format("1/4\nAuthenticating{}", std::string(dotsCount, '.'));
        m_timer.expires_after(std::chrono::milliseconds(600));

        m_timer.async_wait([this] (const auto&){ Task(); });
    }

    StatusWindow::StatusWindow() : m_io_service({}), m_work(m_io_service), m_timer(m_io_service), m_renderTimer(m_io_service)
    {
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
        glfwWindowHint(GLFW_VISIBLE, false);
        glfwWindowHint(GLFW_DECORATED, false);
        glfwWindowHint(GLFW_FLOATING, true);

        m_window = glfwCreateWindow(512, 192, xorstr_("NCC Loader"), nullptr, nullptr);

        SetWindowLong(glfwGetWin32Window(m_window), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
        glfwSetWindowAttrib(m_window, GLFW_MOUSE_PASSTHROUGH, true);

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_window, (mode->width - 512) / 2, (mode->height - 192) / 2);

        glfwMakeContextCurrent(m_window);

        m_context = ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(xorstr_("#version 460"));

        ImFontConfig cfg;
        cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;

        ImGui::GetIO().Fonts->AddFontFromFileTTF(xorstr_(R"(C:\Windows\Fonts\verdanab.ttf)"), 16.f, &cfg);

        LoadTextureFromMemory(rawData, sizeof(rawData), &m_logoTexture,0, 0);

        glfwShowWindow(m_window);
    }
}
