#include "Application.h"
 #include "IndexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
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
    unsigned int shaderProgram;
    GLuint VAO;
    bool wireFrame = false;

    std::unique_ptr<Engine::Texture> texture = std::make_unique<Engine::Texture>("../../../assets/noir.png");
    // std::shared_ptr<Engine::Texture> texture = Engine::Texture::GetWhiteTexture();


    std::shared_ptr<Engine::VertexArray> vertexArray;
    std::shared_ptr<Engine::IndexBuffer> indexBuffer;
    std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
public:
    DummyLayer() {

        // vertex shader
        std::string vsSrc = R"(#version 410 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;
        layout (location = 2) in vec2 aTexCoord;

        out vec4 vColor;
        out vec2 vTexCoord;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vColor = aColor;
            vTexCoord = aTexCoord;
        })";
        const char* vsSrcPtr = vsSrc.c_str();

        unsigned int vs;
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsSrcPtr, nullptr);
        glCompileShader(vs);

        int  success;
        char infoLog[512];
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(vs, 512, nullptr, infoLog);
            LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n {}", infoLog);
        }

        // fragment shader
        std::string fsSrc = R"(#version 410 core
        out vec4 FragColor;
        in vec4 vColor;
        in vec2 vTexCoord;

        uniform sampler2D uTexture;

        void main()
        {
            FragColor = texture(uTexture, vTexCoord) * vColor;
        })";
        const char* fsSrcPtr = fsSrc.c_str();

        unsigned int fs;
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsSrcPtr, nullptr);
        glCompileShader(fs);

        int  success2;
        char infoLog2[512];
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success2);

        if(!success2)
        {
            glGetShaderInfoLog(fs, 512, nullptr, infoLog2);
            LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n {}", infoLog2);
        }

        int success3;
        char infoLog3[512];

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);

        if(!success3) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog3);
            LOG_ERROR("ERROR::SHADER::SHADER_PROGRAM::COMPILATION_FAILED\n {}", infoLog3);
        }
        glDeleteShader(vs);
        glDeleteShader(fs);

        // VAO
        vertexArray = std::make_shared<Engine::VertexArray>();

        // VBO
        vertexBuffer = std::make_shared<Engine::VertexBuffer>(vertices.data(), sizeof(float) * vertices.size());
        vertexBuffer->SetLayout({
            {Engine::ShaderDataType::Float3, "aPosition"},
            {Engine::ShaderDataType::Float4, "aColor"},
            {Engine::ShaderDataType::Float2, "aTexCoord"},
        });
        vertexArray->AddVertexBuffer(vertexBuffer);

        // EBO
        indexBuffer = std::make_shared<Engine::IndexBuffer>(indices.data(),static_cast<unsigned int>(indices.size()));
        vertexArray->SetIndexBuffer(indexBuffer);


    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);
        glUseProgram(shaderProgram);
        texture->Bind(0);
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES,vertexArray->GetIndexBuffer()->Getcount(), GL_UNSIGNED_INT, 0);
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