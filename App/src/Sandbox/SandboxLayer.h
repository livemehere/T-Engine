#pragma once


#include "OrthographicCamera.h"
#include "Texture.h"
#include "Layer.h"
#include "Application.h"

class SandboxLayer : public Engine::Layer {
public:
    SandboxLayer(std::shared_ptr<Engine::OrthographicCamera> camera);
    ~SandboxLayer() override = default;

    void OnUpdate(float dt) override;
    void OnRender() override;

private:
    bool wireFrame = false;

    std::shared_ptr<Engine::OrthographicCamera> camera;
    std::shared_ptr<Engine::Texture> texture;
    std::shared_ptr<Engine::Texture> texture2;
    float cameraMoveSpeed = 400.0f;
    float zoomStep = 0.1f;
    float minZoom = 0.1f;
    float targetZoom = 1.0f;
    float zoomLerpFactor = 0.15f;
};
