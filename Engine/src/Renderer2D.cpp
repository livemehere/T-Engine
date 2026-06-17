#include "Renderer2D.h"

#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "Resource/AssetManager.h"

namespace Engine {

    static constexpr uint32_t TEXTURE_SLOT_COUNT = 16;
    static constexpr uint32_t MAX_QUAD_COUNT = 10000;
    static constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUAD_COUNT * 4;
    static constexpr uint32_t MAX_QUAD_INDICES = MAX_QUAD_COUNT * 6;

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 TexCoord;
        int textureIndex;
    };

    struct Renderer2DData {
        Renderer2D::Statistics stats;

        std::unique_ptr<VertexArray> quadVAO;
        std::shared_ptr<VertexBuffer> quadVBO;
        Shader* quadShader = nullptr;
        std::unique_ptr<Texture> whiteTexture;

        uint32_t quadIndexCount = 0; // total indices count of current frame
        std::unique_ptr<QuadVertex[]> quadVertexBuffers; // starting point
        QuadVertex* quadVertexBufferPtr = nullptr; // current cursor

        int quadViewProjectionLoc = -1;
        int quadTexturesLoc = -1;

        // texture
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

    void Renderer2D::Init() {
        s_storage = std::make_unique<Renderer2DData>();

        // -- Quad start --
        s_storage->quadVAO = std::make_unique<VertexArray>();
        // VBO
        s_storage->quadVertexBuffers =  std::make_unique<QuadVertex[]>(MAX_QUAD_VERTICES);
        auto quadVBO = std::make_shared<VertexBuffer>(nullptr, sizeof(QuadVertex) * MAX_QUAD_VERTICES);
        quadVBO->SetLayout({
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float4, "aColor"},
            {ShaderDataType::Float2, "aTexCoord"},
            {ShaderDataType::Int, "aTextureIndex"},
        });
        s_storage->quadVBO = std::move(quadVBO);
        s_storage->quadVAO->AddVertexBuffer(s_storage->quadVBO);

        // EBO
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
        auto indexBuffer = std::make_shared<IndexBuffer>(quadIndices,MAX_QUAD_INDICES);
        s_storage->quadVAO->SetIndexBuffer(indexBuffer);
        delete[] quadIndices;
        // -- Quad end --

        // shader setup
        s_storage->quadShader = AssetManager::AddShader("Renderer2D.Quad","shaders/Quad.vert","shaders/Quad.frag");
        s_storage->quadShader->Bind();
        s_storage->quadViewProjectionLoc =  glGetUniformLocation(s_storage->quadShader->GetId(), "uViewProjection");
        s_storage->quadTexturesLoc = glGetUniformLocation(s_storage->quadShader->GetId(), "uTextures");

        // textures setup
        unsigned char whitePixel[] = { 255, 255, 255, 255 };
        s_storage->whiteTexture = std::make_unique<Texture>(whitePixel, 1,1,4);
        s_storage->textureSlots.resize(TEXTURE_SLOT_COUNT);
        s_storage->textureSlots[0] = s_storage->whiteTexture.get();

        int samplers[TEXTURE_SLOT_COUNT];
        for (int i=0; i<TEXTURE_SLOT_COUNT;i++) {
            samplers[i] = i;
        }
        glUniform1iv( s_storage->quadTexturesLoc, TEXTURE_SLOT_COUNT, samplers);

    }

    void Renderer2D::Shutdown() {
        s_storage.reset();
    }

    void Renderer2D::BeginScene(const glm::mat4& viewProjection) {
        // shader
        s_storage->quadShader->Bind();

        // camera
        glUniformMatrix4fv(s_storage->quadViewProjectionLoc, 1, GL_FALSE, glm::value_ptr(viewProjection));

        StartBatch();
    }


    void Renderer2D::EndScene() {
        Flush();
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float rotationDeg) {
        m_DrawQuad(position, size, color, s_storage->whiteTexture.get(), rotationDeg);
    }

    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
          Texture* texture, const glm::vec4 &tintColor, float rotationDeg) {
        m_DrawQuad(position, size, tintColor, texture, rotationDeg);
    }

    void Renderer2D::StartBatch() {
        s_storage->quadIndexCount = 0;
        s_storage->quadVertexBufferPtr = s_storage->quadVertexBuffers.get();
        s_storage->textureSlotIndex = 1;
    }

    void Renderer2D::Flush() {
        if (s_storage->quadIndexCount == 0) return;

        const auto dataSize = reinterpret_cast<uint8_t *>(s_storage->quadVertexBufferPtr) - reinterpret_cast<uint8_t *>(
                                                  s_storage->quadVertexBuffers.get());
        s_storage->quadVBO->SetData(s_storage->quadVertexBuffers.get(), dataSize);

        // only used texture slots activate
        for (int i=0; i<s_storage->textureSlotIndex; i++) {
            if (s_storage->textureSlots[i] != nullptr) {
                s_storage->textureSlots[i]->Bind(i);
            }
        }

        s_storage->quadVAO->Bind();
        s_storage->quadShader->Bind();
        glDrawElements(GL_TRIANGLES,s_storage->quadIndexCount, GL_UNSIGNED_INT, 0);
        s_storage->stats.drawCalls++;
    }


    void Renderer2D::m_DrawQuad(const glm::vec2 &position, const glm::vec2 &size,
        const glm::vec4 &color, Texture* texture, float rotationDeg) {
        if (s_storage->quadIndexCount + 6 > MAX_QUAD_INDICES) {
            Flush();
            StartBatch();
        }

        int textureIndex = 0;
        for (int i = 1; i < TEXTURE_SLOT_COUNT; i++) {
            // cache hit
            if (s_storage->textureSlots[i] == texture) {
                textureIndex = i;
                break;
            }
        }

        // new texture
        if (textureIndex == 0) {
            if (s_storage->textureSlotIndex >= TEXTURE_SLOT_COUNT) {
                Flush();
                StartBatch();
            }

            textureIndex = s_storage->textureSlotIndex;
            s_storage->textureSlots[textureIndex] = texture;
            s_storage->textureSlotIndex++;
        }

        // transform
        // pivot
        glm::vec2 pivot(0.5f, 0.5f);
        glm::vec3 pivotOffset = glm::vec3((0.5f - pivot.x) * size.x, (0.5f - pivot.y) * size.y, 0.0f);

        // model
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) // position (x,y)
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotationDeg),glm::vec3(0.0f, 0.0f, 1.0f)) // rotation
        * glm::translate(glm::mat4(1.0f), pivotOffset)
        * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // scale

        static glm::vec4 vertexBasePositions[4] = {
            { -0.5f, -0.5f, 0.0f, 1.0f }, // BL
            {  0.5f, -0.5f, 0.0f, 1.0f }, // BR
            {  0.5f,  0.5f, 0.0f, 1.0f }, // TR
            { -0.5f,  0.5f, 0.0f, 1.0f }  // TL
        };

        static glm::vec2 textureCoords[4] = {
            { 0.0f, 0.0f }, // BL
            { 1.0f, 0.0f }, // BR
            { 1.0f, 1.0f }, // TR
            { 0.0f, 1.0f }  // TL
        };

        for (size_t i = 0; i < 4; i++) {
            s_storage->quadVertexBufferPtr->position = transform * vertexBasePositions[i];
            s_storage->quadVertexBufferPtr->color = color;
            s_storage->quadVertexBufferPtr->TexCoord = textureCoords[i];
            s_storage->quadVertexBufferPtr->textureIndex = textureIndex;
            s_storage->quadVertexBufferPtr++;
        }

        s_storage->quadIndexCount += 6;
        s_storage->stats.quadCount++;

    }
}
