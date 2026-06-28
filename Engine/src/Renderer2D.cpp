#include "Renderer2D.h"

#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "Resource/AssetManager.h"


namespace Engine {

    static constexpr uint32_t TEXTURE_SLOT_COUNT = 16;
    static constexpr uint32_t MAX_QUAD_COUNT = 10000;
    static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUAD_COUNT * 4;
    static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUAD_COUNT * 6;

    // Vertex order must match the shared index pattern: BL, BR, TR, TL.
    static const glm::vec4 VERTEX_BASE_POSITIONS[4] = {
        { -0.5f, -0.5f, 0.0f, 1.0f }, // BL: model-space bottom-left
        {  0.5f, -0.5f, 0.0f, 1.0f }, // BR: model-space bottom-right
        {  0.5f,  0.5f, 0.0f, 1.0f }, // TR: model-space top-right
        { -0.5f,  0.5f, 0.0f, 1.0f }  // TL: model-space top-left
    };

    static const glm::vec2 TEXTURE_COORDS[4] = {
        { 0.0f, 0.0f }, // BL: texture lower-left
        { 1.0f, 0.0f }, // BR: texture lower-right
        { 1.0f, 1.0f }, // TR: texture upper-right
        { 0.0f, 1.0f }  // TL: texture upper-left
    };

    static const glm::vec3 CIRCLE_LOCAL_POSITIONS[4] = {
        { -1.0f, -1.0f, 0.0f }, // BL: circle shader local bottom-left
        {  1.0f, -1.0f, 0.0f }, // BR: circle shader local bottom-right
        {  1.0f,  1.0f, 0.0f }, // TR: circle shader local top-right
        { -1.0f,  1.0f, 0.0f }  // TL: circle shader local top-left
    };

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 TexCoord;
        int textureIndex;
    };

    struct CircleVertex {
        glm::vec3 position;
        glm::vec3 localPosition;
        glm::vec4 color;
        glm::vec2 TexCoord;
        int textureIndex;
        float thickness;
        float fade;
    };

    struct Renderer2DData {
        Renderer2D::Statistics stats;

        // Quad
        std::unique_ptr<VertexArray> quadVAO;
        std::shared_ptr<VertexBuffer> quadVBO;
        Shader* quadShader = nullptr;

        uint32_t quadIndexCount = 0; // total indices count of current frame
        std::unique_ptr<QuadVertex[]> quadVertexBuffers; // starting point
        QuadVertex* quadVertexBufferPtr = nullptr; // current cursor

        int quadViewProjectionLoc = -1;
        int quadTexturesLoc = -1;


        // Circle
        std::unique_ptr<VertexArray> circleVAO;
        std::shared_ptr<VertexBuffer> circleVBO;
        Shader* circleShader = nullptr;

        uint32_t circleIndexCount = 0;
        std::unique_ptr<CircleVertex[]> circleVertexBuffers;
        CircleVertex* circleVertexBufferPtr = nullptr;

        int circleViewProjectionLoc = -1;
        int circleTexturesLoc = -1;


        // texture
        std::unique_ptr<Texture> whiteTexture;
        std::vector<Texture*> textureSlots;
        int textureSlotIndex = 1; // 0 = whiteTexture fixed;

    };

    static std::unique_ptr<Renderer2DData> s_storage;

    void Renderer2D::ResetStats() {
        s_storage->stats = {};
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_storage->stats;
    }

    int Renderer2D::GetTextureIndex(Texture* texture) {
        for (int i = 1; i < TEXTURE_SLOT_COUNT; i++) {
            if (s_storage->textureSlots[i] == texture) {
                return i;
            }
        }

        if (s_storage->textureSlotIndex >= TEXTURE_SLOT_COUNT) {
            Flush();
            StartBatch();
        }

        const int textureIndex = s_storage->textureSlotIndex;
        s_storage->textureSlots[textureIndex] = texture;
        s_storage->textureSlotIndex++;
        return textureIndex;
    }

    glm::mat4 Renderer2D::GetTransform(const glm::vec2& position, const glm::vec2& size, float rotationDeg) {
        glm::vec2 pivot(0.5f, 0.5f);
        glm::vec3 pivotOffset = glm::vec3((0.5f - pivot.x) * size.x, (0.5f - pivot.y) * size.y, 0.0f);

        return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f))
        * glm::translate(glm::mat4(1.0f), pivotOffset)
        * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
    }

    void Renderer2D::Init() {
        s_storage = std::make_unique<Renderer2DData>();

        // Shared EBO
        auto* quadIndices = new uint32_t[MAX_QUAD_INDICES];
        uint32_t offset = 0;

        for (uint32_t i=0; i< MAX_QUAD_INDICES; i+=6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
            offset +=4;
        }
        auto quadIB = std::make_shared<IndexBuffer>(quadIndices,MAX_QUAD_INDICES);
        delete[] quadIndices;

        // -- Quad start --
        s_storage->quadVAO = std::make_unique<VertexArray>();
        s_storage->quadVBO = std::make_shared<VertexBuffer>(nullptr, sizeof(QuadVertex) * MAX_QUAD_VERTICES);
        s_storage->quadVBO->SetLayout({
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float4, "aColor"},
            {ShaderDataType::Float2, "aTexCoord"},
            {ShaderDataType::Int, "aTextureIndex"},
        });
        s_storage->quadVAO->AddVertexBuffer(s_storage->quadVBO);
        s_storage->quadVAO->SetIndexBuffer(quadIB);
        s_storage->quadVertexBuffers =  std::make_unique<QuadVertex[]>(MAX_QUAD_VERTICES);
        // -- Quad end --

        // -- Circle start --
        s_storage->circleVAO = std::make_unique<VertexArray>();
        s_storage->circleVBO = std::make_shared<VertexBuffer>(nullptr, sizeof(CircleVertex) * MAX_QUAD_VERTICES);
        s_storage->circleVBO->SetLayout({
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float3, "aLocalPos"},
            {ShaderDataType::Float4, "aColor"},
            {ShaderDataType::Float2, "aTexCoord"},
            {ShaderDataType::Int, "aTextureIndex"},
            {ShaderDataType::Float, "aThickness"},
            {ShaderDataType::Float, "aFade"},
        });
        s_storage->circleVAO->AddVertexBuffer(s_storage->circleVBO);
        s_storage->circleVAO->SetIndexBuffer(quadIB);
        s_storage->circleVertexBuffers = std::make_unique<CircleVertex[]>(MAX_QUAD_VERTICES);
        // -- Circle end --

        // shader setup
        s_storage->quadShader = AssetManager::AddShader("Renderer2D.Quad","shaders/Quad.vert","shaders/Quad.frag");
        s_storage->quadShader->Bind();
        s_storage->quadViewProjectionLoc =  glGetUniformLocation(s_storage->quadShader->GetId(), "uViewProjection");
        s_storage->quadTexturesLoc = glGetUniformLocation(s_storage->quadShader->GetId(), "uTextures");

        s_storage->circleShader = AssetManager::AddShader("Renderer2D.Circle","shaders/Circle.vert","shaders/Circle.frag");
        s_storage->circleShader->Bind();
        s_storage->circleViewProjectionLoc =  glGetUniformLocation(s_storage->circleShader->GetId(), "uViewProjection");
        s_storage->circleTexturesLoc = glGetUniformLocation(s_storage->circleShader->GetId(), "uTextures");

        // textures setup
        unsigned char whitePixel[] = { 255, 255, 255, 255 };
        s_storage->whiteTexture = std::make_unique<Texture>(whitePixel, 1,1,4);
        s_storage->textureSlots.resize(TEXTURE_SLOT_COUNT);
        s_storage->textureSlots[0] = s_storage->whiteTexture.get();

        int samplers[TEXTURE_SLOT_COUNT];
        for (int i=0; i<TEXTURE_SLOT_COUNT;i++) {
            samplers[i] = i;
        }

        s_storage->quadShader->Bind();
        glUniform1iv( s_storage->quadTexturesLoc, TEXTURE_SLOT_COUNT, samplers);

        s_storage->circleShader->Bind();
        glUniform1iv( s_storage->circleTexturesLoc, TEXTURE_SLOT_COUNT, samplers);

    }

    void Renderer2D::Shutdown() {
        s_storage.reset();
    }

    void Renderer2D::BeginScene(const glm::mat4& viewProjection) {
        s_storage->quadShader->Bind();
        glUniformMatrix4fv(s_storage->quadViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

        s_storage->circleShader->Bind();
        glUniformMatrix4fv(s_storage->circleViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

        StartBatch();
    }

    void Renderer2D::EndScene() {
        Flush();
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float rotationDeg) {
        DrawQuad(position, size, s_storage->whiteTexture.get(), color, rotationDeg);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
          Texture* texture, const glm::vec4 &tintColor, float rotationDeg) {
        if (s_storage->quadIndexCount + 6 > MAX_QUAD_INDICES) {
            Flush();
            StartBatch();
        }

        const int textureIndex = GetTextureIndex(texture);
        const glm::mat4 transform = GetTransform(position, size, rotationDeg);

        for (size_t i = 0; i < 4; i++) {
            s_storage->quadVertexBufferPtr->position = transform * VERTEX_BASE_POSITIONS[i];
            s_storage->quadVertexBufferPtr->color = tintColor;
            s_storage->quadVertexBufferPtr->TexCoord = TEXTURE_COORDS[i];
            s_storage->quadVertexBufferPtr->textureIndex = textureIndex;
            s_storage->quadVertexBufferPtr++;
        }

        s_storage->quadIndexCount += 6;
        s_storage->stats.quadCount++;
    }

    void Renderer2D::DrawCircle(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float thickness, float fade, float rotationDeg) {
        DrawCircle(position, size, s_storage->whiteTexture.get(), color, thickness, fade, rotationDeg);
    }

    void Renderer2D::DrawCircle(const glm::vec2 &position, const glm::vec2 &size,
          Texture* texture, const glm::vec4 &tintColor, float thickness, float fade, float rotationDeg) {
        if (s_storage->circleIndexCount + 6 > MAX_QUAD_INDICES) {
            Flush();
            StartBatch();
        }

        const int textureIndex = GetTextureIndex(texture);

        const glm::mat4 transform = GetTransform(position, size, rotationDeg);

        for (size_t i = 0; i < 4; i++) {
            s_storage->circleVertexBufferPtr->position = transform * VERTEX_BASE_POSITIONS[i];
            s_storage->circleVertexBufferPtr->localPosition = CIRCLE_LOCAL_POSITIONS[i];
            s_storage->circleVertexBufferPtr->color = tintColor;
            s_storage->circleVertexBufferPtr->TexCoord = TEXTURE_COORDS[i];
            s_storage->circleVertexBufferPtr->textureIndex = textureIndex;
            s_storage->circleVertexBufferPtr->thickness = thickness;
            s_storage->circleVertexBufferPtr->fade = fade;
            s_storage->circleVertexBufferPtr++;
        }

        s_storage->circleIndexCount += 6;
        s_storage->stats.quadCount++;
    }

    void Renderer2D::DrawLine(const glm::vec2 &p1, const glm::vec2 &p2, const glm::vec4 &color, float thickness) {
        const glm::vec2 delta = p2 - p1;
        const float length = glm::length(delta);
        if (length <= 0.0f || thickness <= 0.0f) {
            return;
        }

        const glm::vec2 center = (p1 + p2) * 0.5f;
        const float rotationDeg = glm::degrees(std::atan2(delta.y, delta.x));
        DrawQuad(center, { length, thickness }, color, rotationDeg);
    }

    void Renderer2D::DrawQuadOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float thickness, float rotationDeg) {
        if (size.x <= 0.0f || size.y <= 0.0f || thickness <= 0.0f) {
            return;
        }

        const glm::mat4 transform = GetTransform(position, size, rotationDeg);
        glm::vec2 corners[4];
        for (size_t i = 0; i < 4; i++) {
            corners[i] = glm::vec2(transform * VERTEX_BASE_POSITIONS[i]);
        }

        DrawLine(corners[0], corners[1], color, thickness);
        DrawLine(corners[1], corners[2], color, thickness);
        DrawLine(corners[2], corners[3], color, thickness);
        DrawLine(corners[3], corners[0], color, thickness);
    }

    void Renderer2D::DrawCircleOutline(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float thickness, float fade, float rotationDeg) {
        if (size.x <= 0.0f || size.y <= 0.0f || thickness <= 0.0f) {
            return;
        }

        DrawCircle(position, size, color, thickness, fade, rotationDeg);
    }

    void Renderer2D::StartBatch() {
        s_storage->textureSlotIndex = 1;

        s_storage->quadIndexCount = 0;
        s_storage->quadVertexBufferPtr = s_storage->quadVertexBuffers.get();

        s_storage->circleIndexCount = 0;
        s_storage->circleVertexBufferPtr = s_storage->circleVertexBuffers.get();
    }

    void Renderer2D::Flush() {
        // only used texture slots activate
        for (int i=0; i<s_storage->textureSlotIndex; i++) {
            if (s_storage->textureSlots[i] != nullptr) {
                s_storage->textureSlots[i]->Bind(i);
            }
        }

        if (s_storage->quadIndexCount) {
            const auto dataSize = reinterpret_cast<uint8_t *>(s_storage->quadVertexBufferPtr) - reinterpret_cast<uint8_t *>(
                                                      s_storage->quadVertexBuffers.get());
            s_storage->quadVBO->SetData(s_storage->quadVertexBuffers.get(), dataSize);
            s_storage->quadVAO->Bind();
            s_storage->quadShader->Bind();
            glDrawElements(GL_TRIANGLES,s_storage->quadIndexCount, GL_UNSIGNED_INT, 0);
            s_storage->stats.drawCalls++;
        }

        if (s_storage->circleIndexCount) {
            const auto dataSize = reinterpret_cast<uint8_t *>(s_storage->circleVertexBufferPtr) - reinterpret_cast<uint8_t *>(
                                                      s_storage->circleVertexBuffers.get());
            s_storage->circleVBO->SetData(s_storage->circleVertexBuffers.get(), dataSize);
            s_storage->circleVAO->Bind();
            s_storage->circleShader->Bind();
            glDrawElements(GL_TRIANGLES,s_storage->circleIndexCount, GL_UNSIGNED_INT, 0);
            s_storage->stats.drawCalls++;
        }
    }


}
