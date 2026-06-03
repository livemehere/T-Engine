#include "Application.h"
#include "Texture.h"
#include "Window.h"

class DummyLayer : public Engine::Layer {
    unsigned int shaderProgram;
    GLuint VAO;
    bool wireFrame = false;
    int indicesSize = 0;
    Texture texture{"../../../assets/noir.png"};
    // Texture texture = Texture::GetWhiteTexture();

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

        // xyz + rgba
        std::vector<float> vertices = {
            // xyz                    // rgba                      // UV
            -0.5f, 0.5f, 0.0f,        1.0f, 0.0f, 0.0f, 1.0f,      0.0f, 1.0f, // TL
            0.5f, 0.5f, 0.0f,         1.0f, 1.0f, 0.0f, 1.0f,      1.0f, 1.0f, // TR
            -0.5f, -0.5f, 0.0f,       0.0f, 1.0f, 0.0f, 1.0f,      0.0f, 0.0f, // BL
            0.5f, -0.5f, 0.0f,        0.0f, 0.0f, 1.0f, 1.0f,      1.0f, 0.0f, // BR
        };
        int stride = 9;

        std::vector<unsigned int> indices = {
            0,1,2,
            1,2,3
        };
        indicesSize = indices.size();

        // VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        // EBO
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        // xyz
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // rgba
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // UV
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(7*sizeof(float)));
        glEnableVertexAttribArray(2);

    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {
        glPolygonMode(GL_FRONT_AND_BACK,wireFrame ? GL_LINE : GL_FILL);
        texture.Bind();
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,indicesSize, GL_UNSIGNED_INT, 0);
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