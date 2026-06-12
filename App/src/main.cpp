#include "Application.h"

#include "OrthographicCamera.h"
#include "Sandbox/GuiLayer.h"
#include "Sandbox/SandboxLayer.h"

int main() {
    try {
        Engine::AppSpec appSpec{
            .windowSpec = {
                .title = "Sample Window",
                .width = 1920,
                .height = 1080
            }
        };

        Engine::Application app{appSpec};
        auto* window = app.GetWindow();
        auto camera = std::make_shared<Engine::OrthographicCamera>(
            0.0f,
            static_cast<float>(window->GetWidth()),
            0.0f,
            static_cast<float>(window->GetHeight())
        );

        app.PushLayer<SandboxLayer>(camera);
        app.PushLayer<GuiLayer>(camera);
        app.Run();

        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Application crashed: {}", e.what());
        return 1;
    }
}
