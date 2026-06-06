#include "Window.h"


namespace Engine {
    Window::Window(const WindowSpec& spec) {
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


        window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

        if (!window) {
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            glfwDestroyWindow(window);
            window = nullptr;
            throw std::runtime_error("Failed to load GLAD");
        }

        LogOpenGLInfo();

        glfwSetWindowUserPointer(window, this);

        // for device pixel ratio
        glfwGetFramebufferSize(window, &m_frameBufferWidth, &m_frameBufferHeight);
        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
        glfwSwapInterval(m_vsync ? 1 : 0);
    }

    Window::~Window() {
        if (window != nullptr) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
    }

    void Window::Update() const {
        glfwSwapBuffers(window);
    }

    void Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
        const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        glViewport(0,0,width,height);
        LOG_INFO("Viewport Changed : {}x{}",width,height);
        self->m_frameBufferWidth = width;
        self->m_frameBufferHeight = height;
    }

    void Window::LogOpenGLInfo() {
        const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

        LOG_INFO("OpenGL Version: {}", glVersion);
        LOG_INFO("GLSL Version: {}", glslVersion);
        LOG_INFO("GPU Renderer: {}", renderer);
        LOG_INFO("GPU Vendor: {}", vendor);
    }
}