#include "../include/Renderer2D.h"

#include "Shader.h"
#include "VertexArray.h"

namespace Engine {

    struct RectVertex {
        glm::vec3 position;
        glm::vec2 TexCoord;
    };

    struct Renderer2DStorage {
        Renderer2D::Statistics stats;
        std::shared_ptr<Engine::VertexArray> rectVAO;
        std::shared_ptr<Engine::Shader> textureShader;
        std::shared_ptr<Engine::Texture> whiteTexture;
    };

    static std::vector<RectVertex> rectVertices = {
        // Position                // TexCoord
        { {-0.5f,  0.5f, 0.0f},    {0.0f, 1.0f} }, // TL
        { { 0.5f,  0.5f, 0.0f},    {1.0f, 1.0f} }, // TR
        { {-0.5f, -0.5f, 0.0f},    {0.0f, 0.0f} }, // BL
        { { 0.5f, -0.5f, 0.0f},    {1.0f, 0.0f} }  // BR
    };

    static std::vector<unsigned int> rectIndices = {
        0,2,1,
        1,2,3
    };

    static Renderer2DStorage* s_storage;

    void Renderer2D::ResetStats() {
        memset(&s_storage->stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_storage->stats;
    }

    void Renderer2D::Init() {
        s_storage = new Renderer2DStorage();

        // #### Rect Start ####
        s_storage->rectVAO = std::make_shared<Engine::VertexArray>();

        // VBO
        auto vertexBuffer = std::make_shared<Engine::VertexBuffer>(rectVertices.data(), sizeof(RectVertex) * rectVertices.size());
        vertexBuffer->SetLayout({
            {Engine::ShaderDataType::Float3, "aPos"},
            {Engine::ShaderDataType::Float2, "aTexCoord"},
        });
        s_storage->rectVAO->AddVertexBuffer(vertexBuffer);

        // EBO
        auto indexBuffer = std::make_shared<Engine::IndexBuffer>(rectIndices.data(),static_cast<unsigned int>(rectIndices.size()));
        s_storage->rectVAO->SetIndexBuffer(indexBuffer);

        // 1x1 white Texture
        unsigned char whitePixel[] = { 255, 255, 255, 255 };
        s_storage->whiteTexture = std::make_shared<Texture>(whitePixel, 1,1,4);

        // shader
        s_storage->textureShader = std::make_shared<Engine::Shader>("../../../assets/shaders/Texture.vert","../../../assets/shaders/Texture.frag");
        // #### Rect End ####
    }

    void Renderer2D::Shutdown() {
        delete s_storage;
    }

    void Renderer2D::BeginScene(const Engine::OrthographicCamera &camera) {
        // shader
        s_storage->textureShader->Bind();

        // camera
        int vpLoc = glGetUniformLocation(s_storage->textureShader->GetId(), "uViewProjection");
        glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));
    }

    void Renderer2D::EndScene() {
        // TODO: Batch Rendering Flush
    }

    // Rect with solid color
    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color,
        float rotationDeg) {
        m_DrawRect(position, size, color, s_storage->whiteTexture, rotationDeg);
    }

    // Rect with texture & color
    void Renderer2D::DrawRect(const glm::vec2 &position, const glm::vec2 &size,
         const glm::vec4 &color, const std::shared_ptr<Engine::Texture> &texture, float rotationDeg) {
        m_DrawRect(position, size, color, texture, rotationDeg);
    }

    // Rect Base
    void Renderer2D::m_DrawRect(const glm::vec2 &position, const glm::vec2 &size,
         const glm::vec4 &color, const std::shared_ptr<Engine::Texture> &texture, float rotationDeg) {
        s_storage->textureShader->Bind();
        texture->Bind();

        // color
        int colorLoc = glGetUniformLocation(s_storage->textureShader->GetId(), "uColor");
        glUniform4fv(colorLoc,1, glm::value_ptr(color));

        // transform
        // pivot
        glm::vec2 pivot(0.5f, 0.5f);
        glm::vec3 pivotOffset = glm::vec3((0.5f - pivot.x) * size.x, (0.5f - pivot.y) * size.y, 0.0f);

        // model
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) // position (x,y)
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotationDeg),glm::vec3(0.0f, 0.0f, 1.0f)) // rotation
        * glm::translate(glm::mat4(1.0f), pivotOffset)
        * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f)); // scale

        int transformLoc = glGetUniformLocation(s_storage->textureShader->GetId(), "uTransform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        s_storage->rectVAO->Bind();
        glDrawElements(GL_TRIANGLES,s_storage->rectVAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

        s_storage->stats.drawCalls++;
        s_storage->stats.rectCount++;

    }
} // Engine