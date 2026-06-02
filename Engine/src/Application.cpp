#include "Application.h"

namespace Engine {
    Application* Application::s_instance = nullptr;

    Application::Application(const AppSpec &spec) {

        if (s_instance != nullptr) {
            LOG_ERROR("Can not instantiate Application more than one");
            exit(1);
        }

        if (!glfwInit()) {
            LOG_ERROR("Fail to init GLFW");
            exit(1);
        }
        glfwSetErrorCallback(ErrorCallback);
        m_window = std::make_unique<Window>(spec.windowSpec);
        s_instance = this;
    }

    Application::~Application() {
        s_instance = nullptr;
    }

    void Application::Run() const {

        float lastTime = static_cast<float>(glfwGetTime());

        while (!m_window->ShouldClose()) {
            glfwPollEvents();

            const float time = static_cast<float>(glfwGetTime());
            const float dt = time - lastTime;
            lastTime = time;

            for (auto& layer : m_layerStack) {
                layer->OnUpdate(dt);
            }

            // NOTE: can be elsewhere like thread
            for (auto& layer : m_layerStack) {
                layer->OnRender();
            }

            m_window->Update();
        }

    }




    void Application::ErrorCallback(int error, const char *description) {
        LOG_ERROR("GLFW ERROR : {}", description);
    }
}
