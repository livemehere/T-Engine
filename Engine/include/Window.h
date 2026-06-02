#pragma once

#include "EngineCommon.h"

struct WindowSpec {
    std::string title;
    int width;
    int height;
    bool vsync = true;
};


class Window {
public:
    Window(const WindowSpec& spec);
     ~Window();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetFrameBufferWidth() const { return frameBufferWidth; }
    int GetFrameBufferHeight() const { return frameBufferHeight; }
    GLFWwindow* GetNativeWindow() const { return window; }

private:
    std::string title;
    int width;
    int height;
    int frameBufferWidth;
    int frameBufferHeight;
    bool vsync = true;

    GLFWwindow* window = nullptr;

    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void ErrorCallback(int error, const char* description);
    static void LogOpenGLInfo();
};
