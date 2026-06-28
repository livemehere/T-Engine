#include "SandboxLayer.h"

#include <imgui.h>

#include "Resource/AssetManager.h"
#include "Renderer2D.h"
#include "Camera/OrthographicCamera.h"
#include "Core/Input.h"

SandboxLayer::SandboxLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera)
    : Layer(name), m_camera(std::move(camera)), m_cameraController(m_camera) {

    texture = Engine::AssetManager::AddTexture("noir", "textures/noir.png");
    texture2 = Engine::AssetManager::AddTexture("spider", "textures/spider.png");

    glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);

}

void SandboxLayer::OnUpdate(float dt) {
    m_cameraController.OnUpdate(dt);
}

void SandboxLayer::OnRender() {
    auto window = Engine::Application::Get().GetWindow();
    float width = static_cast<float>(window->GetWidth());
    float height = static_cast<float>(window->GetHeight());

    auto white = glm::vec4{255.0f, 255.0f,255.0f, 1.0f};
    auto red = glm::vec4{255.0f, 0.0f,0.0f, 1.0f};

    Engine::Renderer2D::BeginScene(m_camera->GetViewProjection());

    // XY-Axis
    Engine::Renderer2D::DrawLine({0, 1000.0f}, {0, -1000.0f}, {255.0f, 0.0f,0.0f, 1.0f});
    Engine::Renderer2D::DrawLine({1000.0f, 0.0f}, {-1000.0f, 0.0f}, {0.0f, 255.0f,0.0f, 1.0f});

    // rect
    Engine::Renderer2D::DrawQuad({0.0f, 0.0f}, {50,50}, white);
    Engine::Renderer2D::DrawQuadOutline({0.0f, 0.0f}, {50,50}, red);

    // circle
    Engine::Renderer2D::DrawCircle({100.0f, 0.0f}, {50.0f, 50.0f},white);
    Engine::Renderer2D::DrawCircleOutline({100.0f, 0.0f}, {50.0f, 50.0f},red);

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
