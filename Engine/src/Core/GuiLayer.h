#pragma once
#include "Layer.h"

namespace Engine {
    class GuiLayer : public Layer {
    public:
        GuiLayer() : Layer("_GUI_Layer") {}
        ~GuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();
    };
}
