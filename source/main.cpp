//
// Created by Orange on 1/12/2025.
//
#include <cstdio>
#include <CodeVirtualizer/VirtualizerSDK.h>
#include <CodeVirtualizer/StealthCodeArea.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include "gui/StatusWindow.hpp"
#include "boost/asio.hpp"
#include <boost/asio/thread_pool.hpp>
#include <boost/thread.hpp>
#include <xorstr.hpp>
#include <tray.hpp>

#include "stb_image.h"
#include "images/ncc_ico.h"
#include <windows.h>

// Function to create an HICON from raw PNG data
HICON CreateHICONFromPNG(const unsigned char* pngData, size_t pngSize) {
    int width, height, channels;

    // Decode PNG data
    unsigned char* imageData = stbi_load_from_memory(pngData, static_cast<int>(pngSize), &width, &height, &channels, 4);
    if (!imageData) {
        return nullptr;
    }

    // Convert RGBA to BGRA (Swap R and B channels)
    for (int i = 0; i < width * height * 4; i += 4) {
        std::swap(imageData[i], imageData[i + 2]);  // Swap Red and Blue
    }

    // Prepare BITMAPV5HEADER
    BITMAPV5HEADER bmh = {};
    bmh.bV5Size = sizeof(BITMAPV5HEADER);
    bmh.bV5Width = width;
    bmh.bV5Height = -height;  // Negative for top-down DIB
    bmh.bV5Planes = 1;
    bmh.bV5BitCount = 32;
    bmh.bV5Compression = BI_BITFIELDS;
    bmh.bV5RedMask = 0x00FF0000;
    bmh.bV5GreenMask = 0x0000FF00;
    bmh.bV5BlueMask = 0x000000FF;
    bmh.bV5AlphaMask = 0xFF000000;

    // Create a device-independent bitmap (DIB)
    HDC hdc = GetDC(nullptr);
    void* dibPixels = nullptr;
    HBITMAP hBitmap = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO*>(&bmh), DIB_RGB_COLORS, &dibPixels, nullptr, 0);
    ReleaseDC(nullptr, hdc);

    if (!hBitmap || !dibPixels) {
        stbi_image_free(imageData);
        return nullptr;
    }

    // Copy pixel data into the DIB
    memcpy(dibPixels, imageData, width * height * 4);

    // Free stb_image memory
    stbi_image_free(imageData);

    // Create an empty mask bitmap (not used, but required by CreateIconIndirect)
    HBITMAP hMask = CreateBitmap(width, height, 1, 1, NULL);

    // ICONINFO structure
    ICONINFO ii = {};
    ii.fIcon = TRUE;
    ii.hbmColor = hBitmap;
    ii.hbmMask = hMask;

    // Create the HICON
    const HICON цhIcon = CreateIconIndirect(&ii);

    // Cleanup
    DeleteObject(hBitmap);
    DeleteObject(hMask);

    return hIcon;
}

void CVSX(unsigned int i) {};
void CvStealthArea()
{
    //const auto CVSX = [](unsigned int) {};
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
    VIRTUALIZER_MUTATE_ONLY_START

    if (FindWindowA(nullptr, xorstr_("NCC Loader")))
    {
        MessageBoxA(nullptr, xorstr_("Nullcore loader has already been started!"), xorstr_("Error"), MB_OK | MB_ICONERROR);

        return EXIT_FAILURE;
    }

    std::thread([]()
    {
        auto ico = CreateHICONFromPNG(ncc_ico, sizeof(ncc_ico));

        Tray::Tray tray(xorstr_("NCC Tray"), ico);

        tray.addEntry(Tray::Button(xorstr_("Inject"), [&]{
          tray.exit();
        }));
        tray.addEntry(Tray::Separator());
        tray.addEntry(Tray::Button(xorstr_("Info"), [&]{
          MessageBoxA(nullptr, xorstr_("License info"), xorstr_("NCC loader"), MB_OK | MB_ICONINFORMATION);
        }));
        tray.addEntry(Tray::Separator());
        tray.addEntry(Tray::Button(xorstr_("Exit"), [&]{
          exit(EXIT_SUCCESS);
        }));
        tray.run();
    }).detach();


    if (!glfwInit())
        std::exit(EXIT_FAILURE);

    ncc_loader::gui::StatusWindow status_window;

    boost::asio::post(status_window.m_io_service, [&](){status_window.RunOnce();});
    boost::asio::post(status_window.m_io_service, [&](){status_window.Task();});

    status_window.m_io_service.run();

    if (reinterpret_cast<uintptr_t>(&main) == 0x1111)
        CvStealthArea();


    VIRTUALIZER_MUTATE_ONLY_END
    return EXIT_SUCCESS;
}