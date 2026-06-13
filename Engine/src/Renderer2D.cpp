#include "Renderer2D.h"

#include "Resource/Shader.h"
#include "VertexArray.h"

namespace Engine {

    static constexpr unsigned int TEXTURE_SAMPLES_COUNT = 16;
    static constexpr unsigned int MAX_RECT_COUNT = 10000;
    static constexpr unsigned int MAX_VERTICES = MAX_RECT_COUNT * 4;
    static constexpr unsigned int MAX_INDICES = MAX_RECT_COUNT * 6;

    struct RectVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 TexCoord;
        float textureIndex;
    };

    struct Renderer2DStorage {
        Renderer2D::Statistics stats;
        std::shared_ptr<VertexArray> rectVAO;
        std::shared_ptr<VertexBuffer> rectVBO;
        std::shared_ptr<Shader> textureShader;
        std::unique_ptr<Texture> whiteTexture;

        unsigned int rectIndexCount = 0; // total indices count of current frame
        RectVertex* rectVertexBufferBase = nullptr; // starting point
        RectVertex* rectVertexBufferPtr = nullptr; // current cursor

        // texture
        std::vector<const Texture*> textureSlots;
        float textureSlotIndex = 1.0f; // 0 = whiteTexture fixed;

    };

    static Renderer2DStorage* s_storage;

    void Renderer2D::ResetStats() {
        memset(&s_storage->stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_storage->stats;
    }

    void Renderer2D::Init() {
        if (s_storage != nullptr) {
            return;
        }

        s_storage = new Renderer2DStorage();

        // #### Rect Start ####
        s_storage->rectVAO = std::make_shared<VertexArray>();

        // #### VBO START ####
        s_storage->rectVertexBufferBase = new RectVertex[MAX_VERTICES];
        auto vertexBuffer = std::make_shared<VertexBuffer>(nullptr, sizeof(RectVertex) * MAX_VERTICES);
        vertexBuffer->SetLayout({
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float4, "aColor"},
            {ShaderDataType::Float2, "aTexCoord"},
            {ShaderDataType::Float, "aTextureIndex"},
        });
        s_storage->rectVBO = vertexBuffer;
        s_storage->rectVAO->AddVertexBuffer(vertexBuffer);
        // #### VBO END ####

        // #### EBO START ####
        unsigned int* rectIndices = new unsigned int[MAX_INDICES];
        unsigned int offset = 0;

        for (unsigned int i=0; i< MAX_INDICES; i+=6) {
            rectIndices[i + 0] = offset + 0;
            rectIndices[i + 1] = offset + 1;
            rectIndices[i + 2] = offset + 2;

            rectIndices[i + 3] = offset + 2;
            rectIndices[i + 4] = offset + 3;
            rectIndices[i + 5] = offset + 0;
            offset +=4;
        }
        auto indexBuffer = std::make_shared<IndexBuffer>(rectIndices,MAX_INDICES);
        s_storage->rectVAO->SetIndexBuffer(indexBuffer);
        delete[] rectIndices;
        // #### EBO END ####


        // 1x1 white Texture
        unsigned char whitePixel[] = { 255, 255, 255, 255 };
        s_storage->whiteTexture = std::make_unique<Texture>(whitePixel, 1,1,4);

        // texture slots
        s_storage->textureSlots.resize(TEXTURE_SAMPLES_COUNT);
        s_storage->textureSlots[0] = s_storage->whiteTexture.get();

        // shader
        s_storage->textureShader = std::make_shared<Shader>("../../../assets/shaders/Texture.vert","../../../assets/shaders/Texture.frag");
        s_storage->textureShader->Bind();
        // #### Rect End ####

        // texture
        int samplers[TEXTURE_SAMPLES_COUNT];
        for (int i=0; i<TEXTURE_SAMPLES_COUNT;i++) {
            samplers[i] = i;
        }
        int texturesLoc = glGetUniformLocation(s_storage->textureShader->GetId(), "uTextures");
        glUniform1iv(texturesLoc, TEXTURE_SAMPLES_COUNT, samplers);

    }

    void Renderer2D::Shutdown() {
        if (s_storage == nullptr) {
            return;
        }

        delete[] s_storage->rectVertexBufferBase;
        delete s_storage;
        s_storage = nullptr;
    }

    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        // shader
        s_storage->textureShader->Bind();

        // camera
        int vpLoc = glGetUniformLocation(s_storage->textureShader->GetId(), "uViewProjection");
        glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));

        StartBatch();
    }


    void Renderer2D::EndScene() {
        Flush();
    }

    // Rect with solid color
    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float rotationDeg) {
        m_DrawRect(position, size, color, s_storage->whiteTexture.get(), rotationDeg);
    }

    // Rect with texture & color
    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size,
         const glm::vec4 &color, const Texture* texture, float rotationDeg) {
        m_DrawRect(position, size, color, texture, rotationDeg);
    }

    void Renderer2D::StartBatch() {
        s_storage->rectIndexCount = 0;
        s_storage->rectVertexBufferPtr = s_storage->rectVertexBufferBase;
        // texture
        s_storage->textureSlotIndex = 1.0f;
    }

    void Renderer2D::Flush() {
        if (s_storage->rectIndexCount == 0) return;

        uint32_t dataSize = (uint32_t)((uint8_t*)s_storage->rectVertexBufferPtr - (uint8_t*)s_storage->rectVertexBufferBase);
        s_storage->rectVBO->SetData(s_storage->rectVertexBufferBase, dataSize);

        for (int i=0; i<(int)s_storage->textureSlotIndex; i++) {
            if (s_storage->textureSlots[i] != nullptr) {
                s_storage->textureSlots[i]->Bind(i);
            }
        }

        s_storage->rectVAO->Bind();
        s_storage->textureShader->Bind();
        glDrawElements(GL_TRIANGLES,s_storage->rectIndexCount, GL_UNSIGNED_INT, 0);
        s_storage->stats.drawCalls++;
    }


    // Rect Base
    void Renderer2D::m_DrawRect(const glm::vec2 &position, const glm::vec2 &size,
        const glm::vec4 &color, const Texture* texture, float rotationDeg) {
        if (s_storage->rectIndexCount >= MAX_INDICES) {
            Flush();
            StartBatch();
        }

        float textureIndex = 0.0f;
        if (texture == nullptr) {
            texture = s_storage->whiteTexture.get();
        }

        for (int i = 1; i < TEXTURE_SAMPLES_COUNT; i++) {
            // re-use
            if (s_storage->textureSlots[i] == texture) {
                textureIndex = (float)i;
                break;
            }
        }

        // new texture
        if (textureIndex == 0.0f) {
            if (s_storage->textureSlotIndex >= TEXTURE_SAMPLES_COUNT) {
                Flush();
                StartBatch();
            }

            textureIndex = s_storage->textureSlotIndex;
            s_storage->textureSlots[(int)textureIndex] = texture;
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
            s_storage->rectVertexBufferPtr->position = transform * vertexBasePositions[i];
            s_storage->rectVertexBufferPtr->color = color;
            s_storage->rectVertexBufferPtr->TexCoord = textureCoords[i];
            s_storage->rectVertexBufferPtr->textureIndex = textureIndex;
            s_storage->rectVertexBufferPtr++;
        }

        s_storage->rectIndexCount += 6;
        s_storage->stats.rectCount++;

    }
} // Engine
