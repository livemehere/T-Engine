#include "Application.h"
#include "Window.h"

class DummyLayer : public Engine::Layer {
    void OnUpdate(float dt) override {
        LOG_INFO("dt {}", dt);
    }

    void OnRender() override {
        LOG_INFO("Render! {}", Engine::Application::Get().GetWindow()->GetTitle());
    }
};

int main() {

    Engine::AppSpec appSpec{
        .windowSpec = {
            .title = "Sample Window",
            .width = 1920,
            .height = 1080
        }
    };

    Engine::Application app{appSpec};
    app.PushLayer<DummyLayer>();
    app.Run();

    return 0;
}