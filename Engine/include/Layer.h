#pragma once

#include "EngineCommon.h"

namespace Engine {
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void OnUpdate(float dt) {}
        virtual void OnRender(){}
    };
}

