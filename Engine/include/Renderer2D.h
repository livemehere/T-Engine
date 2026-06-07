#pragma once

#include "EngineCommon.h"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Engine {
    class Renderer2D {
    public:
        struct Statistics {
            unsigned int drawCalls = 0;
            unsigned int rectCount = 0;
        };
        static void ResetStats();
        static Statistics GetStats();

        static void Init();
        static void Shutdown();

        static void BeginScene(const Engine::OrthographicCamera& camera);
        static void EndScene();

        // solid
        static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);

        // texture
        static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Engine::Texture>& texture, float rotationDeg = 0.0f);
    private:
        static void m_DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Engine::Texture>& texture, float rotationDeg = 0.0f);
    };

} // Engine
