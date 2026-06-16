#include "Application.h"
#include "Resource/AssetManager.h"
#include "Renderer2D.h"

namespace Engine {
    void Application::PopLayer(Layer *layer) {
        m_layerStack.PopLayer(layer);
    }

    void Application::PopOverlay(Layer *layer) {
        m_layerStack.PopOverlay(layer);
    }

    Application* Application::s_instance = nullptr;

    Application::Application(const AppSpec &spec) {
        AssetManager::SetAssetRoot(spec.assetRoot);

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

        PushOverlay<GuiLayer>();
    }

    Application::~Application() {
        m_layerStack.Clear();

        Renderer2D::Shutdown();
        AssetManager::ClearTextureAll();

        m_window.reset();
        s_instance = nullptr;
        glfwTerminate();
    }

    void Application::Run() {

        Renderer2D::Init();

        double lastTime = glfwGetTime();

        // NOTE: 2D zIndex handle by transform z sort, not use GL_DEPTH_TEST, this cause PNG Alpha clipping bug.
        // glEnable(GL_DEPTH_TEST);

        while (!m_window->ShouldClose()) {
            Renderer2D::ResetStats();

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


            m_guiLayer.Begin();
            for (const auto& layer : m_layerStack) {
                layer->OnGuiRender();
            }
            m_guiLayer.End();

            m_layerStack.PlushPendingCommands();

            m_window->Update();
        }
    }

    void Application::ErrorCallback(int error, const char *description) {
        LOG_ERROR("GLFW ERROR {}: {}", error, description);
    }
}
