#pragma once

#include "Camera/OrthographicCamera.h"
#include "Camera/OrthographicCameraController.h"
#include "Resource/Texture.h"
#include "Layer/Layer.h"
#include "Core/Application.h"

class SandboxLayer : public Engine::Layer {
public:
    SandboxLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera);
    ~SandboxLayer() override = default;

    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnAttach() override;
    void OnDetach() override;
    void OnGuiRender() override;

private:
    bool wireFrame = false;

    std::shared_ptr<Engine::OrthographicCamera> m_camera;
    Engine::OrthographicCameraController m_cameraController;
    Engine::Texture* texture;
    Engine::Texture* texture2;

};
