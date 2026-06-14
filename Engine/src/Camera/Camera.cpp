#include "Camera.h"

namespace Engine {
    void Camera::SetPosition(const glm::vec3& position) {
        m_position = position;
        ReCalculateView();
    }
    void Camera::SetRotation(const glm::vec3& rotation) {
        m_rotation = rotation;
        ReCalculateView();
    }

    void Camera::ReCalculateView() {

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position)
        // NOTE: fix gimbal lock to convert quat?
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(1.0f, 0.0f, 1.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        m_view = glm::inverse(transform);
        ReCalculateViewProjection();
    }

    void Camera::ReCalculateViewProjection() {
        m_viewProjection = m_projection * m_view;
    }
}
