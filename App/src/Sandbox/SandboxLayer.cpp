#include "SandboxLayer.h"

#include <imgui.h>

#include "Resource/AssetManager.h"
#include "Renderer2D.h"
#include "Camera/OrthographicCamera.h"
#include "Core/Input.h"

SandboxLayer::SandboxLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera)
    : Layer(name), m_camera(std::move(camera)), m_cameraController(m_camera) {

    const auto window = Engine::Application::Get().GetWindow();
    const auto width = static_cast<float>(window->GetWidth());
    const auto height = static_cast<float>(window->GetHeight());

    m_camera->SetPosition({
        -width/2.0f,
        -height/2.0f,
        0.0f
    });

    texture = Engine::AssetManager::AddTexture("noir", "textures/noir.png");
    texture2 = Engine::AssetManager::AddTexture("spider", "textures/spider.png");

    glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);

}

void SandboxLayer::OnUpdate(float dt) {
    m_cameraController.OnUpdate(dt);
}

void SandboxLayer::OnRender() {

    auto white = glm::vec4{1.0f,1.0f, 1.0f, 1.0f};
    auto red   = glm::vec4{1.0f,0.0f,0.0f,1.0f};
    auto green = glm::vec4{0.0f,1.0f,0.0f,1.0f};

    Engine::Renderer2D::BeginScene(m_camera->GetViewProjection());

    // XY-Axis
    Engine::Renderer2D::DrawLine({0.0f, 1000.0f}, {0.0f, -1000.0f}, red, 1.0f);
    Engine::Renderer2D::DrawLine({1000.0f, 0.0f}, {-1000.0f, 0.0f}, green, 1.0f);

    static float deg = 0.0f;
    deg += 1.0f;

    const float primitiveY = 220.0f;
    // Quad
    Engine::Renderer2D::DrawQuad({-300.0f, primitiveY}, {50.0f, 50.0f},texture, white, deg);
    Engine::Renderer2D::DrawQuadOutline({-300.0f, primitiveY}, {50.0f, 50.0f}, red, 2.0f, deg);

    // Circle
    Engine::Renderer2D::DrawCircle({-200.0f, primitiveY}, {50.0f, 50.0f},texture, white, 1.0f, 0.005f, deg);
    Engine::Renderer2D::DrawCircleOutline({-200.0f, primitiveY}, {50.0f, 50.0f}, red, 1.0f, 0.005f, deg);

    // Triangle
    Engine::Renderer2D::DrawTriangle({-100.0f, primitiveY}, {50.0f, 50.0f},texture, white, deg);
    Engine::Renderer2D::DrawTriangleLine({-100.0f, primitiveY}, {50.0f, 50.0f}, red, 2.0f, deg);

    // Polygon (same ratio)
    const glm::vec2 start = {-300.0f, 80.0f};
    const glm::vec2 cellGap = {110.0f, -110.0f};
    const glm::vec2 polygonSize = {60.0f, 60.0f};

    const int columns = 5;
    const int firstSide = 3;
    const int lastSide = 12;

    for (int side = firstSide; side <= lastSide; side++) {
        const int index = side - firstSide;

        const int col = index % columns;
        const int row = index / columns;

        const glm::vec2 position = {
            start.x + static_cast<float>(col) * cellGap.x,
            start.y + static_cast<float>(row) * cellGap.y
        };

        Engine::Renderer2D::DrawPolygon(
            position,
            polygonSize,
            static_cast<uint32_t>(side),
            texture,
            white,
            deg
        );

        Engine::Renderer2D::DrawPolygonLine(
            position,
            polygonSize,
            static_cast<uint32_t>(side),
            red,
            2.0f,
            deg
        );
    }

    // Polygon (diff ratio)
    const glm::vec2 ellipseStart = {-300.0f, -130.0f};

    for (int i = 0; i < 5; i++) {
        const uint32_t sideCount = static_cast<uint32_t>(3 + i * 3);

        const glm::vec2 position = {
            ellipseStart.x + static_cast<float>(i) * 130.0f,
            ellipseStart.y
        };

        Engine::Renderer2D::DrawPolygon(
            position,
            {90.0f, 50.0f},
            sideCount,
            texture,
            white,
            deg
        );

        Engine::Renderer2D::DrawPolygonLine(
            position,
            {90.0f, 50.0f},
            sideCount,
            red,
            2.0f,
            deg
        );
    }

    Engine::Renderer2D::EndScene();
}


void SandboxLayer::OnAttach() {
    LOG_INFO("Attach Sandbox Layer");
}

void SandboxLayer::OnDetach() {
    LOG_INFO("Detach Sandbox Layer");
}

void SandboxLayer::OnGuiRender() {
    auto viewportSize = Engine::Application::Get().GetWindow()->GetSize();
    auto mousePos = Engine::Input::GetMousePosition();
    const glm::vec3 worldCursor = m_camera->ScreenToWorld(mousePos, viewportSize);

    ImGui::Begin("Mouse");
    ImGui::Text("World : %.1f x %.1f", worldCursor.x, worldCursor.y);
    ImGui::End();

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->AddCircle(ImVec2(mousePos.x, mousePos.y), 10.0f * m_camera->GetZoom(), IM_COL32(0, 255, 0, 255), 232, 2.0f);

    ImGui::Begin("Stats");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    auto stats = Engine::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Quad Count: %d", stats.quadCount);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}
