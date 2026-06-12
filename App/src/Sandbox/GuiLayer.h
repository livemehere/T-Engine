#pragma once

#include "Layer.h"
#include "OrthographicCamera.h"

class GuiLayer : public Engine::Layer{
public:
    GuiLayer(std::shared_ptr<Engine::OrthographicCamera> camera);
    ~GuiLayer() override;

    void OnUpdate(float dt) override;
    void OnRender() override;

private:
    std::shared_ptr<Engine::OrthographicCamera> camera;
};
