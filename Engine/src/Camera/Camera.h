#pragma once

#include "EngineCommon.h"

namespace Engine {
    class Camera {
    public:
        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_projection; }
        const glm::mat4& GetView() const { return m_view; }
        const glm::mat4& GetViewProjection() const { return m_viewProjection; }

        const glm::vec3& GetPosition() const { return m_position; }
        void SetPosition(const glm::vec3& position);
        const glm::vec3& GetRotation() const { return m_rotation; }
        void SetRotation(const glm::vec3& rotation);

    protected:
       Camera() = default;

        virtual void ReCalculateProjection() = 0;
        void ReCalculateView();
        void ReCalculateViewProjection();

        glm::mat4 m_projection{1.0f};
        glm::mat4 m_view{1.0f};
        glm::mat4 m_viewProjection{1.0f};

        glm::vec3 m_position{0.0f, 0.0f, 0.0f};
        glm::vec3 m_rotation{0.0f, 0.0f, 0.0f};
    };
}
