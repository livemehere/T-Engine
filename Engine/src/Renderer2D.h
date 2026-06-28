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

        // Fill
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f),  float rotationDeg = 0.0f);

        static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, float rotationDeg = 0.0f);
        static void DrawCircle(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f), float thickness = 1.0f, float fade = 0.005f, float rotationDeg = 0.0f);

        static void DrawTriangle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);
        static void DrawTriangle(const glm::vec2& position, const glm::vec2& size, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f), float rotationDeg = 0.0f);
        static void DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec4& color);
        static void DrawTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f));

        static void DrawPolygon(const glm::vec2& position, const glm::vec2& size,uint32_t sideCount, const glm::vec4& color, float rotationDeg = 0.0f);
        static void DrawPolygon(const glm::vec2& position, const glm::vec2& size,uint32_t sideCount, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f), float rotationDeg = 0.0f);

        // Stroke
        static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec4& color, float thickness = 1.0f);
        static void DrawLine(const glm::vec2& p1, const glm::vec2& p2, Texture* texture, const glm::vec4& tintColor = glm::vec4(1.0f), float thickness = 1.0f);

        static void DrawTriangleLine(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float rotationDeg = 0.0f);
        static void DrawTriangleLine(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec4& color, float thickness = 1.0f);

        static void DrawPolygonLine(const glm::vec2& position, const glm::vec2& size,uint32_t sideCount, const glm::vec4& color, float thickness, float rotationDeg = 0.0f);

        static void DrawQuadLine(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float rotationDeg = 0.0f);

        static void DrawCircleLine(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, float rotationDeg = 0.0f);

    private:
        static void StartBatch();
        static void Flush();

        static int GetTextureIndex(Texture* texture);
        static glm::mat4 GetTransform(const glm::vec2& position, const glm::vec2& size, float rotationDeg);
        static void FillMiterJoin(const glm::vec2& prev, const glm::vec2& current, const glm::vec2& next, const glm::vec4& color, float thickness);
        static void DrawTriangleWithUV(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3, Texture* texture, const glm::vec4& tintColor);

        static void DrawPolygon(const std::vector<glm::vec2>& points, const glm::vec4& color);
        static void DrawPolygon(const std::vector<glm::vec2>& points, Texture* texture, const glm::vec4& tintColor);
        static void DrawPolygonLine(const std::vector<glm::vec2>& points, const glm::vec4& color, float thickness = 1.0f);
        static std::vector<glm::vec2> CreateRegularPolygonPoints(const glm::vec2& position, const glm::vec2& size, uint32_t sideCount,float rotationDeg);
    };

}
