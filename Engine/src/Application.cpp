#include "Application.h"

#include <stdexcept>

#include "AssetManager.h"
#include "Renderer2D.h"

namespace Engine {
    Application* Application::s_instance = nullptr;

    Application::Application(const AppSpec &spec) {

        if (s_instance != nullptr) {
            throw std::runtime_error("Cannot instantiate Application more than once");
        }

        glfwSetErrorCallback(ErrorCallback);

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        s_instance = this;

        try {
            m_window = std::make_unique<Window>(spec.windowSpec);
        } catch (...) {
            s_instance = nullptr;
            glfwTerminate();
            throw;
        }
    }

    Application::~Application() {

        Renderer2D::Shutdown();
        AssetManager::ClearTextureAll();

        m_layerStack.clear();
        m_window.reset();
        s_instance = nullptr;
        glfwTerminate();
    }

    void Application::Run() {

        Renderer2D::Init();

        double lastTime = glfwGetTime();

        while (!m_window->ShouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();

            const double time = glfwGetTime();
            const float dt = static_cast<float>(time - lastTime);
            lastTime = time;

            for (const auto& layer : m_layerStack) {
                layer->OnUpdate(dt);
            }

            // NOTE: can be elsewhere like thread
            for (const auto& layer : m_layerStack) {
                layer->OnRender();
            }

            m_window->Update();
        }
    }

    void Application::ErrorCallback(int error, const char *description) {
        LOG_ERROR("GLFW ERROR {}: {}", error, description);
    }
}
