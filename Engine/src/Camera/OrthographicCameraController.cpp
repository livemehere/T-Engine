#include "OrthographicCameraController.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/KeyCode.h"

namespace Engine {
    OrthographicCameraController::OrthographicCameraController(std::shared_ptr<OrthographicCamera> camera) {
        m_camera = std::move(camera);
    }

    void OrthographicCameraController::OnUpdate(float dt) {
        const float currentZoom = m_camera->GetZoom();
        glm::vec3 position = m_camera->GetPosition();

        const float moveAmount = m_cameraMoveSpeed * dt / currentZoom;

        if (Input::IsKeyPressed(KeyCode::A)) {
            position.x -= moveAmount;
        }
        if (Input::IsKeyPressed(KeyCode::D)) {
            position.x += moveAmount;
        }
        if (Input::IsKeyPressed(KeyCode::S)) {
            position.y -= moveAmount;
        }
        if (Input::IsKeyPressed(KeyCode::W)) {
            position.y += moveAmount;
        }

        m_camera->SetPosition(position);

        const float scrollYOffset = Input::ConsumeScrollYOffset();
        if (scrollYOffset != 0.0f) {
            m_targetZoom = std::max(m_minZoom, m_targetZoom + scrollYOffset * m_zoomStep);
        }

        auto viewportSize = Application::Get().GetWindow()->GetSize();
        auto mousePos = Input::GetMousePosition();

        const glm::vec3 worldCursor = m_camera->ScreenToWorld(mousePos, viewportSize);

        const float nextZoom = glm::mix(currentZoom, m_targetZoom, m_zoomLerpFactor);
        if (std::abs(nextZoom - currentZoom) > 0.0001f) {
            m_camera->ZoomTowards(worldCursor, nextZoom);
        }
    }
}
