#include "Window.h"


namespace Engine {
    Window::Window(const WindowSpec& spec) {
        if (spec.width <= 0 || spec.height <= 0) {
            throw std::runtime_error("Window dimensions must be greater than zero");
        }

        m_title = spec.title;
        m_width = spec.width;
        m_height = spec.height;
        m_vsync = spec.vsync;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        #endif


        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!m_window) {
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
            throw std::runtime_error("Failed to load GLAD");
        }

        LogOpenGLInfo();

        glfwSetWindowUserPointer(m_window, this);

        // for device pixel ratio
        glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);
        glViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);
        glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
        glfwSetScrollCallback(m_window, ScrollCallback);
        glfwSwapInterval(m_vsync ? 1 : 0);
    }

    Window::~Window() {
        if (m_window != nullptr) {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void Window::Update() const {
        glfwSwapBuffers(m_window);
    }

    float Window::ConsumeScrollYOffset() {
        const float scrollYOffset = m_scrollYOffset;
        m_scrollYOffset = 0.0f;
        return scrollYOffset;
    }

    void Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
        const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        glViewport(0,0,width,height);
        LOG_INFO("Viewport Changed : {}x{}",width,height);
        self->m_frameBufferWidth = width;
        self->m_frameBufferHeight = height;
    }

    void Window::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_scrollYOffset += static_cast<float>(yOffset);
    }

    void Window::LogOpenGLInfo() {
        const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

        if (glVersion == nullptr || glslVersion == nullptr || renderer == nullptr || vendor == nullptr) {
            throw std::runtime_error("Failed to query OpenGL driver information");
        }

        LOG_INFO("OpenGL Version: {}", glVersion);
        LOG_INFO("GLSL Version: {}", glslVersion);
        LOG_INFO("GPU Renderer: {}", renderer);
        LOG_INFO("GPU Vendor: {}", vendor);
    }
}
