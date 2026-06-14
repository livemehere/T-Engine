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

        inline static int maxTextureUnits = 0;
        inline static int maxCombinedUnits = 0;

        const std::string& GetTitle() const { return m_title;};
        GLFWwindow* GetHandle() const { return m_window; }

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        int GetFrameBufferWidth() const { return m_frameBufferWidth; }
        int GetFrameBufferHeight() const { return m_frameBufferHeight; }

        float ConsumeScrollYOffset();
        bool ShouldClose() const { return glfwWindowShouldClose(m_window); }
        void Update() const;

    private:
        std::string m_title;
        GLFWwindow* m_window = nullptr;

        int m_width;
        int m_height;
        int m_frameBufferWidth;
        int m_frameBufferHeight;
        bool m_vsync = true;

        float m_scrollYOffset = 0.0f;

        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void CheckTextureLimits();
        static void LogOpenGLInfo();
    };
}
