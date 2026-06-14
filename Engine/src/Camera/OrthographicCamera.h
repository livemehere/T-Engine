#pragma once

#include "Camera.h"

namespace Engine {
    class OrthographicCamera : public Camera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera() override = default;

        void SetProjection(float left, float right, float bottom, float top);

        float GetZoom() const { return m_zoom; }
        void SetZoom(float zoom);
        void ZoomTowards(const glm::vec3& targetPos, float targetZoom);

        glm::vec3 ScreenToWorld(const glm::vec2& screenPosition, const glm::vec2& viewportSize, float ndcZ = 0.0f) const;

        glm::vec2 WorldToScreen(const glm::vec3& worldPosition, const glm::vec2& viewportSize) const ;

    private:
        void ReCalculateProjection() override;
        float m_left = 0.0f;
        float m_right = 0.0f;
        float m_bottom = 0.0f;
        float m_top = 0.0f;

        float m_zoom = 1.0f;
        float m_nearClip = -1.0f;
        float m_farClip = 1.0f;
    };
}
