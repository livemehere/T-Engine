#pragma once

#include "Core/Layer.h"
#include "Core/Window.h"
#include "Core/LayerStack.h"

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

        void Run();

        static Application& Get() {
            assert(s_instance && "[Application] instance is null");
            return *s_instance;
        }
        Window* GetWindow() const { return m_window.get();}

        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        TLayer* PushLayer(Args&&... args) {
            TLayer* layer = m_layerStack.PushLayer<TLayer>(std::forward<Args>(args)...);
            return layer;
        }
        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        TLayer* PushOverlay(Args&&... args) {
            TLayer* layer = m_layerStack.PushOverlay<TLayer>(std::forward<Args>(args)...);
            return layer;
        }

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);
    private:
        static Application* s_instance;

        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;

        static void ErrorCallback(int error, const char *description);
    };
}
