#pragma once

#include "EngineCommon.h"
#include "Resource/Texture.h"

namespace Engine {
    class Renderer2D {
    public:
        struct Statistics {
            unsigned int drawCalls = 0;
            unsigned int quadCount = 0;
        };
        static void ResetStats();
        static Statistics GetStats();

        static void Init();
        static void Shutdown();

        static void BeginScene(const glm::mat4& viewProjection);
        static void EndScene();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f),  float rotationDeg = 0.0f);

        static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, float rotationDeg = 0.0f);
        static void DrawCircle(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f), float thickness = 1.0f, float fade = 0.005f, float rotationDeg = 0.0f);

        static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0f);

    private:
        static void StartBatch();
        static void Flush();

        static int GetTextureIndex(Texture* texture);
        static glm::mat4 GetTransform(const glm::vec2& position, const glm::vec2& size, float rotationDeg);
    };

}
