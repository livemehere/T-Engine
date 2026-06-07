#pragma once

#include "EngineCommon.h"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Engine {
    class Renderer2D {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Engine::OrthographicCamera& camera);
        static void EndScene();

        // solid
        static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);

        // texture
        static void DrawRect(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Engine::Texture>& texture, const glm::vec4& tint, float rotationDeg = 0.0f);
    };
} // Engine
