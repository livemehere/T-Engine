#include "SandboxLayer.h"

#include <imgui.h>

#include "Resource/AssetManager.h"
#include "Renderer2D.h"

SandboxLayer::SandboxLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera)
    : Layer(name), camera(std::move(camera)) {
    targetZoom = this->camera->GetZoom();

    texture = Engine::AssetManager::AddTexture("noir", "textures/noir.png");
    texture2 = Engine::AssetManager::AddTexture("spider", "textures/spider.png");

    glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);

}

void SandboxLayer::OnUpdate(float dt) {
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

    auto screenWidth = static_cast<float>(window->GetWidth());
    auto screenHeight = static_cast<float>(window->GetHeight());

    double screenX = 0.0;
    double screenY = 0.0;
    glfwGetCursorPos(handle, &screenX, &screenY);

    const glm::vec3 worldCursor = camera->ScreenToWorld({screenX, screenY}, {screenWidth, screenHeight});

    const float nextZoom = glm::mix(currentZoom, targetZoom, zoomLerpFactor);
    if (std::abs(nextZoom - currentZoom) > 0.0001f) {
        camera->ZoomTowards(worldCursor, nextZoom);
    }
}

void SandboxLayer::OnRender() {
    Engine::Renderer2D::ResetStats();

    auto window = Engine::Application::Get().GetWindow();
    float width = static_cast<float>(window->GetWidth()) * 5;
    float height = static_cast<float>(window->GetHeight()) * 5;

    static int gap = 5;
    static glm::vec2 size(10.0f, 10.0f);
    static float rotationDeg = 0.0f;
    static glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    const int step = static_cast<int>(size.x) + gap;
    rotationDeg += 1.0f;

    Engine::Renderer2D::BeginScene(camera->GetViewProjectionMatrix());

    for (int x = 0; x < width; x += step) {
        for (int y = 0; y < height; y += step) {
            glm::vec3 position(x + size.x / 2, y + size.y / 2, 2.0f);
            const bool useFirstTexture = ((x / step) + (y / step)) % 2 == 0;
            Engine::Renderer2D::DrawRect(position, size, color, useFirstTexture ? texture : texture2, rotationDeg);
        }
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
    auto window = Engine::Application::Get().GetWindow();
    auto* handle = window->GetHandle();

    auto screenWidth = static_cast<float>(window->GetWidth());
    auto screenHeight = static_cast<float>(window->GetHeight());

    double screenX = 0.0;
    double screenY = 0.0;
    glfwGetCursorPos(handle, &screenX, &screenY);

    const glm::vec3 worldCursor = camera->ScreenToWorld({screenX, screenY}, {screenWidth, screenHeight});

    ImGui::Begin("Mouse");
    ImGui::Text("World : %.1f x %.1f", worldCursor.x, worldCursor.y);
    ImGui::End();

    ImDrawList* drawList = ImGui::GetForegroundDrawList();
    drawList->AddCircle(ImVec2(screenX, screenY), 10.0f * camera->GetZoom(), IM_COL32(0, 255, 0, 255), 232, 2.0f);

    ImGui::Begin("Stats");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

    auto stats = Engine::Renderer2D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.drawCalls);
    ImGui::Text("Rect Count: %d", stats.rectCount);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}
