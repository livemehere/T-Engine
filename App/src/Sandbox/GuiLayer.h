#pragma once

#include "../../../Engine/src/Core/Layer.h"
#include "OrthographicCamera.h"

class GuiLayer : public Engine::Layer{
public:
    GuiLayer(const std::string& name, std::shared_ptr<Engine::OrthographicCamera> camera);
    ~GuiLayer() override;

    void OnUpdate(float dt) override;
    void OnRender() override;
    void OnAttach() override;
    void OnDetach() override;

private:
    std::shared_ptr<Engine::OrthographicCamera> camera;
};
