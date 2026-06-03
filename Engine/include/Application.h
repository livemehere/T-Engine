#pragma once

#include "EngineCommon.h"
#include "Layer.h"
#include "Window.h"

namespace Engine {
    struct AppSpec {
        WindowSpec windowSpec;
    };

    class Application {
    public:
        Application(const AppSpec& spec);
        ~Application();

        void Run() const;

        static Application& Get() {
            if (s_instance == nullptr) {
                throw std::runtime_error("Application instance is not available");
            }

            return *s_instance;
        }
        Window* GetWindow() const { return m_window.get();}

        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer(Args&&... args) {
            m_layerStack.push_back(std::make_unique<TLayer>(std::forward<Args>(args)...));
        }

    private:
        static Application* s_instance;

        std::unique_ptr<Window> m_window;
        std::vector<std::unique_ptr<Layer>> m_layerStack;


        static void ErrorCallback(int error, const char *description);
    };
}
