#include "../../Engine/src/Application.h"
#include "../../Engine/src/IndexBuffer.h"
#include "../../Engine/src/OrthographicCamera.h"
#include "../../Engine/src/Renderer2D.h"
#include "../../Engine/src/Texture.h"
#include "../../Engine/src/Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../../Engine/src/AssetManager.h"

class DummyLayer : public Engine::Layer {
    bool wireFrame = false;

    std::unique_ptr<Engine::OrthographicCamera> camera;
    std::shared_ptr<Engine::Texture> texture;;
    std::shared_ptr<Engine::Texture> texture2;
    float cameraMoveSpeed = 400.0f;
    float zoomStep = 0.1f;
    float minZoom = 0.1f;
    float targetZoom = 1.0f;
    float zoomLerpFactor = 0.15f;
public:
    DummyLayer() {

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());
        camera = std::make_unique<Engine::OrthographicCamera>(0.0f, width, 0.0f, height);
        targetZoom = camera->GetZoom();
        texture = Engine::AssetManager::LoadTexture("noir","../../../assets/noir.png");
        texture2 = Engine::AssetManager::LoadTexture("spider","../../../assets/spider.png");
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);

        Engine::Renderer2D::Init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 410 core");

    }
    ~DummyLayer() = default;

    void OnUpdate(float dt) override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        auto window = Engine::Application::Get().GetWindow();
        auto* handle = window->GetHandle();

        const float currentZoom = camera->GetZoom();

        glm::vec3 position = camera->GetPosition();
        const float moveAmount = cameraMoveSpeed * dt / currentZoom;

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
            targetZoom = std::max(minZoom, targetZoom + scrollYOffset * zoomStep);
        }

        auto screenWidth = (float)window->GetWidth();
        auto screenHeight = (float)window->GetHeight();

        double screenX = 0.0;
        double screenY = 0.0;
        glfwGetCursorPos(handle, &screenX, &screenY);

        const glm::vec3 worldCursor = camera->ScreenToWorld({screenX, screenY},{screenWidth, screenHeight});

        ImGui::Begin("Mouse");
        ImGui::Text("World : %.1f x %.1f", worldCursor.x, worldCursor.y);

        ImGui::End();

        const float nextZoom = glm::mix(currentZoom, targetZoom, zoomLerpFactor);
        if (std::abs(nextZoom - currentZoom) > 0.0001f) {
            camera->ZoomTowards(worldCursor, nextZoom);
        }
    }

    void OnRender() override {
        Engine::Renderer2D::ResetStats();

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth()) * 5;
        float height = static_cast<float>(window->GetHeight()) * 5;

        static int gap = 5;
        static glm::vec2 size(10.0f, 10.0f);
        static float rotationDeg = 0.0f;
        static glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};
        const int step = static_cast<int>(size.x) + gap;
        rotationDeg+= 1.0f;

        ImDrawList* drawList = ImGui::GetForegroundDrawList();

        auto* handle = window->GetHandle();
        double screenX = 0.0;
        double screenY = 0.0;
        glfwGetCursorPos(handle, &screenX, &screenY);
        glm::vec2 circlePos = camera->WorldToScreen({width/5/2, height/5/2, 0.0f}, {width/5, height/5});
        drawList->AddCircle(ImVec2(screenX, screenY),10.0f * camera->GetZoom(), IM_COL32(0,255,0,255), 232, 2.0f);

        Engine::Renderer2D::BeginScene(*camera);

        for (int x = 0; x<width; x+= step) {
            for (int y = 0; y < height; y+= step) {
                glm::vec3 position(x + size.x/2, y + size.y/2, 2.0f);
                const bool useFirstTexture = ((x / step) + (y / step)) % 2 == 0;
                Engine::Renderer2D::DrawRect(position, size, color, useFirstTexture ? texture : texture2, rotationDeg);
            }
        }

        Engine::Renderer2D::EndScene();



        ImGui::Begin("Stats");
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("FSP: %.1f",ImGui::GetIO().Framerate);

        auto stats = Engine::Renderer2D::GetStats();
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Rect Count: %d", stats.rectCount);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
        Engine::AssetManager::Shutdown();

        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Application crashed: {}", e.what());
        return 1;
    }
}
