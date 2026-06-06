#include "Application.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Window.h"

static std::vector<unsigned int> indices = {
    0,2,1,
    1,2,3
};

static std::vector<float> vertices = {
    // xyz                    // rgba                      // UV
    -0.5f, 0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,      0.0f, 1.0f, // TL
    0.5f, 0.5f, 0.0f,         1.0f, 1.0f, 1.0f, 1.0f,      1.0f, 1.0f, // TR
    -0.5f, -0.5f, 0.0f,       1.0f, 1.0f, 1.0f, 1.0f,      0.0f, 0.0f, // BL
    0.5f, -0.5f, 0.0f,        1.0f, 1.0f, 1.0f, 1.0f,      1.0f, 0.0f, // BR
};

class DummyLayer : public Engine::Layer {
    bool wireFrame = false;

    std::unique_ptr<Engine::Texture> texture = std::make_unique<Engine::Texture>("../../../assets/noir.png");
    // std::shared_ptr<Engine::Texture> texture = Engine::Texture::GetWhiteTexture();

    std::shared_ptr<Engine::VertexArray> vertexArray;
    std::shared_ptr<Engine::Shader> shader;
public:
    DummyLayer() {

        shader = std::make_shared<Engine::Shader>("../../../assets/shaders/Texture.vert","../../../assets/shaders/Texture.frag");

        // VAO
        vertexArray = std::make_shared<Engine::VertexArray>();

        // VBO
        auto vertexBuffer = std::make_shared<Engine::VertexBuffer>(vertices.data(), sizeof(float) * vertices.size());
        vertexBuffer->SetLayout({
            {Engine::ShaderDataType::Float3, "aPos"},
            {Engine::ShaderDataType::Float4, "aColor"},
            {Engine::ShaderDataType::Float2, "aTexCoord"},
        });
        vertexArray->AddVertexBuffer(vertexBuffer);

        // EBO
        auto indexBuffer = std::make_shared<Engine::IndexBuffer>(indices.data(),static_cast<unsigned int>(indices.size()));
        vertexArray->SetIndexBuffer(indexBuffer);


    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);
        shader->Bind();
        texture->Bind(0);
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES,vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
    }
};

int main() {
    try {
        Engine::AppSpec appSpec{
            .windowSpec = {
                .title = "Sample Window",
                .width = 1920,
                .height = 1080
            }
        };

        Engine::Application app{appSpec};
        app.PushLayer<DummyLayer>();
        app.Run();

        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL("Application crashed: {}", e.what());
        return 1;
    }
}