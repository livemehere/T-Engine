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
    std::shared_ptr<Engine::Texture> texture2;
    float cameraMoveSpeed = 400.0f;
    float zoomStep = 0.1f;
    float minZoom = 0.1f;
public:
    DummyLayer() {

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());
        camera = std::make_unique<Engine::OrthographicCamera>(0.0f, width, 0.0f, height);
        texture = std::make_shared<Engine::Texture>("../../../assets/noir.png");
        texture2 = std::make_shared<Engine::Texture>("../../../assets/spider.png");
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);

        Engine::Renderer2D::Init();
    }
    ~DummyLayer() = default;

    void OnUpdate(float dt) override {
        auto window = Engine::Application::Get().GetWindow();
        auto* handle = window->GetHandle();

        glm::vec3 position = camera->GetPosition();
        const float moveAmount = cameraMoveSpeed * dt;

        if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS) {
            position.x -= moveAmount;
        }
        if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS) {
            position.x += moveAmount;
        }
        if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS) {
            position.y -= moveAmount;
        }
        if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS) {
            position.y += moveAmount;
        }

        camera->SetPosition(position);

        const float scrollYOffset = window->ConsumeScrollYOffset();
        if (scrollYOffset != 0.0f) {
            const float nextZoom = std::max(minZoom, camera->GetZoom() + scrollYOffset * zoomStep);
            camera->SetZoom(nextZoom);
        }

        auto stats = Engine::Renderer2D::GetStats();
        float fps = 1.0f / dt;
        LOG_INFO("FPS: {:.1f} | draw Calls: {} | rect: {}", fps, stats.drawCalls, stats.rectCount);
    }

    void OnRender() override {
        Engine::Renderer2D::ResetStats();

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());


        // camera->ZoomTowards(glm::vec3(width, height/2,0.0f), glm::mix(camera->GetZoom(), 2.0f, 0.01f));
        // camera->SetZoom(camera->GetZoom() + 0.001f);


        static int gap = 5;
        static glm::vec2 size(10.0f, 10.0f);
        static float rotationDeg = 0.0f;
        static glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};
        rotationDeg+= 1.0f;

        Engine::Renderer2D::BeginScene(*camera);

        for (int x = 0; x<width; x+= size.x + gap) {
            for (int y = 0; y < height; y+= size.y + gap) {
                glm::vec3 position(x + size.x/2, y + size.y/2, 2.0f);
                Engine::Renderer2D::DrawRect(position, size, color, x > width/2 ? texture : texture2, rotationDeg);
            }
        }

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