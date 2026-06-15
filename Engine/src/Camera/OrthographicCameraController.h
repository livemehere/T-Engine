#pragma once

#include <memory>
#include "OrthographicCamera.h"

namespace Engine {
    class OrthographicCameraController {
    public:
        OrthographicCameraController(std::shared_ptr<OrthographicCamera> camera);
        ~OrthographicCameraController() = default;

        void OnUpdate(float dt);

    private:
        std::shared_ptr<OrthographicCamera> m_camera;
        float m_cameraMoveSpeed = 400.0f;

        // zoom
        float m_zoomStep = 0.1f;
        float m_minZoom = 0.1f;
        float m_targetZoom = 1.0f;
        float m_zoomLerpFactor = 0.15f;
    };
}
