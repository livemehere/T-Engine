#include "Application.h"
#include "IndexBuffer.h"
#include "OrthographicCamera.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Window.h"

class DummyLayer : public Engine::Layer {
    bool wireFrame = false;

    std::unique_ptr<Engine::OrthographicCamera> camera;
    std::shared_ptr<Engine::Texture> texture;;
public:
    DummyLayer() {

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());
        camera = std::make_unique<Engine::OrthographicCamera>(0.0f, width, 0.0f, height);
        texture = std::make_shared<Engine::Texture>("../../../assets/noir.png");
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);

        Engine::Renderer2D::Init();

    }
    ~DummyLayer() {
    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());


        // camera->ZoomTowards(glm::vec3(width, height/2,0.0f), glm::mix(camera->GetZoom(), 2.0f, 0.01f));
        // camera->SetZoom(camera->GetZoom() + 0.001f);

        // transform
        glm::vec3 position(width/2, height/2, 0.0f);
        static glm::vec2 size(100.0f, 100.0f);
        static float rotationDeg = 0.0f;
        static glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};
        rotationDeg+= 0.1f;
        // scale.x += 10.0f;

        Engine::Renderer2D::BeginScene(*camera);

        Engine::Renderer2D::DrawRect(position, size, color, rotationDeg);

        Engine::Renderer2D::EndScene();





    }
};

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
        app.PushLayer<DummyLayer>();
        app.Run();

        Engine::Renderer2D::Shutdown();

        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Application crashed: {}", e.what());
        return 1;
    }
}