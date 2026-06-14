#include "OrthographicCamera.h"

namespace Engine {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) : m_left(left), m_right(right), m_bottom(bottom), m_top(top) {
        ReCalculateView();
        ReCalculateProjection();
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
        m_left = left;
        m_right = right;
        m_bottom = bottom;
        m_top = top;
        ReCalculateProjection();
    }

    void OrthographicCamera::SetZoom(float zoom) {
        if (zoom <= 0.0f) {
            return;
        }

        m_zoom = zoom;
        ReCalculateProjection();
    }

    void OrthographicCamera::ZoomTowards(const glm::vec3 &targetPos, float targetZoom) {
        if (m_zoom == 0.0f || targetZoom == 0.0f) return;

        float zoomRatio = m_zoom / targetZoom;

        glm::vec3 distance = targetPos - m_position;
        glm::vec3 newPosition = targetPos - distance * zoomRatio;

        SetZoom(targetZoom);
        SetPosition(newPosition);
    }

    glm::vec3 OrthographicCamera::ScreenToWorld(const glm::vec2 &screenPosition, const glm::vec2 &viewportSize,
        float ndcZ) const {
        float ndcX =  2.0f * (screenPosition.x / viewportSize.x) - 1.0f;
        float ndcY = 1.0f - 2.0f * (screenPosition.y / viewportSize.y);

        glm::vec4 ndcCoords{ndcX, ndcY, ndcZ, 1.0f};
        glm::mat4 inverseVP = glm::inverse(m_viewProjection);
        glm::vec4 worldPos = inverseVP * ndcCoords;
        return glm::vec3(worldPos);
    }

    glm::vec2 OrthographicCamera::WorldToScreen(const glm::vec3 &worldPosition, const glm::vec2 &viewportSize) const {
        glm::vec4 ndc = m_viewProjection * glm::vec4(worldPosition, 1.0f);

        float screenX = ((ndc.x + 1.0f) * 0.5f) * viewportSize.x;
        float screenY = ((1.0f - ndc.y ) * 0.5f) * viewportSize.y;

        return glm::vec2(screenX, screenY);
    }

    void OrthographicCamera::ReCalculateProjection() {
        m_projection = glm::ortho(
            m_left / m_zoom,
            m_right / m_zoom,
            m_bottom / m_zoom,
            m_top / m_zoom,
            m_nearClip,
            m_farClip
        );
        ReCalculateViewProjection();
    }
}