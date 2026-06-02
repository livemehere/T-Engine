#pragma once

#include "EngineCommon.h"

namespace Engine {
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

        std::string GetTitle() const { return m_title;};
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        int GetFrameBufferWidth() const { return m_frameBufferWidth; }
        int GetFrameBufferHeight() const { return m_frameBufferHeight; }
        GLFWwindow* GetHandle() const { return window; }

        bool ShouldClose() const { return glfwWindowShouldClose(window); }
        void Update() const;

    private:
        std::string m_title;
        int m_width;
        int m_height;
        int m_frameBufferWidth;
        int m_frameBufferHeight;
        bool m_vsync = true;

        GLFWwindow* window = nullptr;

        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        static void ErrorCallback(int error, const char* description);
        static void LogOpenGLInfo();
    };
}
