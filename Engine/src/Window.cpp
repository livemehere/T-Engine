#include "Window.h"

Window::Window(const WindowSpec& spec) {
    title = spec.title;
    width = spec.width;
    height = spec.height;
    vsync = spec.vsync;

    if (!glfwInit()) {
        LOG_ERROR("Fail to init GLFW");
        exit(1);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        LOG_ERROR("Fail to create window");
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwTerminate();
        LOG_ERROR("Fail to load GLAD");
        exit(1);
    }

    LogOpenGLInfo();

    glfwSetWindowUserPointer(window, this);

    // for device pixel ratio
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    glfwSwapInterval(vsync ? 1 : 0);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::FrameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    const auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    glViewport(0,0,width,height);
    LOG_INFO("Viewport Changed : {}x{}",width,height);
    self->frameBufferWidth = width;
    self->frameBufferHeight = height;
}

void Window::ErrorCallback(int error, const char *description) {
    LOG_ERROR("GLFW ERROR : {}", description);
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
