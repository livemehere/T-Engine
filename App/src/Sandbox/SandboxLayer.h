#pragma once


#include "OrthographicCamera.h"
#include "Resource/Texture.h"
#include "Core/Layer.h"
#include "Core/Application.h"

class SandboxLayer : public Engine::Layer {
public:
    SandboxLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera);
    ~SandboxLayer() override = default;

    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnAttach() override;
    void OnDetach() override;

private:
    bool wireFrame = false;

    std::shared_ptr<Engine::OrthographicCamera> camera;
    const Engine::Texture* texture;
    const Engine::Texture* texture2;
    float cameraMoveSpeed = 400.0f;
    float zoomStep = 0.1f;
    float minZoom = 0.1f;
    float targetZoom = 1.0f;
    float zoomLerpFactor = 0.15f;
};
