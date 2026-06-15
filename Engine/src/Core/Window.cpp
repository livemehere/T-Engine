#include "Window.h"

#include "Input.h"

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
        glfwSwapInterval(m_vsync ? 1 : 0);

        // update initial viewport
        glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);
        glViewport(0, 0, m_frameBufferWidth, m_frameBufferHeight);

        // callbacks
        glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
        glfwSetFramebufferSizeCallback(m_window, FrameBufferSizeCallback);
        glfwSetScrollCallback(m_window, ScrollCallback);
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

    void Window::WindowSizeCallback(GLFWwindow* window, int width, int height) {
        const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->m_width = width;
        self->m_height = height;
        LOG_INFO("size change {}x{}",self->m_width, self->m_height);
    }

    void Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
        const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        glViewport(0,0,width,height);
        self->m_frameBufferWidth = width;
        self->m_frameBufferHeight = height;
        LOG_INFO("Viewport Changed : {}x{}",self->m_frameBufferWidth,self->m_frameBufferHeight);
    }

    void Window::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
        Input::AddMouseScrollOffset(xOffset, yOffset);
    }

    void Window::CheckTextureLimits() {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Window::maxTextureUnits);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &Window::maxCombinedUnits);

        LOG_INFO("MAX Texture units: {}",Window::maxTextureUnits);
        LOG_INFO("MAX combined Texture units: {}",Window::maxCombinedUnits);
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

        CheckTextureLimits();
    }
}
