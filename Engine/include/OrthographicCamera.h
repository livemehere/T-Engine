#pragma once

#include "EngineCommon.h"

// 🔔header only
namespace Engine {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top): m_BoundsLeft(left), m_BoundsRight(right), m_BoundsBottom(bottom), m_BoundsTop(top), m_viewMatrix(1.0f) {
            RecalculateProjectionMatrix();
        }
        ~OrthographicCamera() = default;

        void SetProjection(float left, float right, float bottom, float top){
            m_BoundsLeft = left;
            m_BoundsRight = right;
            m_BoundsBottom = bottom;
            m_BoundsTop = top;
            RecalculateProjectionMatrix();
        }

        float GetZoom() const { return m_zoom;}
        void SetZoom(float zoom) {
            m_zoom = zoom;
            RecalculateProjectionMatrix();
        }

        // transform
        const glm::vec3& GetPosition() const { return m_position;}
        void SetPosition(const glm::vec3& position){
            m_position = position;
            RecalculateViewMatrix();
        }

        float GetRotation() const { return m_rotation;}
        void SetRotation(float rotation) {
            m_rotation = rotation;
            RecalculateViewMatrix();
        }

        const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix;}
        const glm::mat4& GetViewMatrix() const { return m_viewMatrix;}
        const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix;}


        void ZoomTowards(const glm::vec3& targetPos, float targetZoom) {
            float oldZoom = m_zoom;

            if (oldZoom == 0.0f) return;

            float zoomRatio =   oldZoom / targetZoom;

            glm::vec3 oldPos = m_position;

            glm::vec3 newPos = targetPos - (targetPos - oldPos) * zoomRatio;

            SetZoom(targetZoom);
            SetPosition(newPos);
        }

    private:
        void RecalculateViewMatrix(){
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            m_viewMatrix = glm::inverse(transform);
            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        void RecalculateProjectionMatrix() {
            m_projectionMatrix = glm::ortho(
                m_BoundsLeft / m_zoom,
                m_BoundsRight / m_zoom,
                m_BoundsBottom / m_zoom,
                m_BoundsTop / m_zoom,
                -1.0f,
                1.0f
            );

            m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        }

        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix;

        glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
        float m_rotation = 0.0f;

        float m_BoundsLeft, m_BoundsRight, m_BoundsBottom, m_BoundsTop;
        float m_zoom = 1.0f;
    };
}
