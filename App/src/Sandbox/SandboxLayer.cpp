#include "SandboxLayer.h"

#include "AssetManager.h"
#include "Renderer2D.h"

SandboxLayer::SandboxLayer(std::shared_ptr<Engine::OrthographicCamera> camera)
    : camera(std::move(camera)) {
    targetZoom = this->camera->GetZoom();

    texture = Engine::AssetManager::AddTexture("noir", "../../../assets/noir.png");
    texture2 = Engine::AssetManager::AddTexture("spider", "../../../assets/spider.png");

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

    Engine::Renderer2D::BeginScene(*camera);

    for (int x = 0; x < width; x += step) {
        for (int y = 0; y < height; y += step) {
            glm::vec3 position(x + size.x / 2, y + size.y / 2, 2.0f);
            const bool useFirstTexture = ((x / step) + (y / step)) % 2 == 0;
            Engine::Renderer2D::DrawRect(position, size, color, useFirstTexture ? texture : texture2, rotationDeg);
        }
    }

    Engine::Renderer2D::EndScene();
}
