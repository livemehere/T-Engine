#include "Application.h"
#include "IndexBuffer.h"
#include "OrthographicCamera.h"
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

    std::unique_ptr<Engine::OrthographicCamera> camera;
    std::unique_ptr<Engine::Texture> texture;;
    std::shared_ptr<Engine::VertexArray> vertexArray;
    std::shared_ptr<Engine::Shader> shader;
public:
    DummyLayer() {

        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());
        camera = std::make_unique<Engine::OrthographicCamera>(0.0f, width, 0.0f, height);

        texture = std::make_unique<Engine::Texture>("../../../assets/noir.png");
        // texture = Engine::Texture::GetWhiteTexture();

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

        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);
    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {
        shader->Bind();
        texture->Bind(0);

        int vpLoc = glGetUniformLocation(shader->GetId(), "uViewProjection");
        glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewProjectionMatrix()));


        auto window = Engine::Application::Get().GetWindow();
        float width = static_cast<float>(window->GetWidth());
        float height = static_cast<float>(window->GetHeight());

        // transform
        glm::vec3 position(width/2, height/2, 0.0f);
        static glm::vec2 scale(800.0f, 600.0f);
        static float rotationDeg = 0.0f;
        rotationDeg+= 0.1f;
        // scale.x += 10.0f;

        // pivot
        glm::vec2 pivot(0.5f, 0.5f);
        glm::vec3 pivotOffset = glm::vec3((0.5f - pivot.x) * scale.x, (0.5f - pivot.y) * scale.y, 0.0f);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) // position (x,y)
        * glm::rotate(glm::mat4(1.0f),glm::radians(rotationDeg),glm::vec3(0.0f, 0.0f, 1.0f)) // rotation
        * glm::translate(glm::mat4(1.0f), pivotOffset)
        * glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f)); // scale

        int transformLoc = glGetUniformLocation(shader->GetId(), "uTransform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

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