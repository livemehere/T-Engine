#pragma once

#include "Core/Layer.h"
#include "Core/Window.h"

namespace Engine {
    struct AppSpec {
        WindowSpec windowSpec;
    };

    class Application {
    public:
        Application(const AppSpec& spec);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(const Application&&) = delete;
        Application& operator=(Application&&) = delete;

        void Run() const;

        static Application& Get() {
            assert(s_instance && "[Application] instance is null");
            return *s_instance;
        }
        Window* GetWindow() const { return m_window.get();}

        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        void PushLayer(Args&&... args) {
            auto layer = std::make_unique<TLayer>(std::forward<Args>(args)...);
            layer->OnAttach();
            m_layerStack.push_back(std::move(layer));
        }

    private:
        static Application* s_instance;

        std::unique_ptr<Window> m_window;
        std::vector<std::unique_ptr<Layer>> m_layerStack;

        static void ErrorCallback(int error, const char *description);
    };
}
