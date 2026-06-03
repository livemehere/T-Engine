#include "Application.h"
#include "Window.h"

class DummyLayer : public Engine::Layer {
    unsigned int shaderProgram;
    GLuint VAO;
public:
    DummyLayer() {

        // vertex shader
        std::string vsSrc = R"(#version 410 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aColor;

        out vec4 vColor;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            vColor = aColor;
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

        void main()
        {
            FragColor = vColor;
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
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top / r
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// left / g
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f// right / b
        };

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // xyz
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // rgba
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void OnUpdate(float dt) override {
    }

    void OnRender() override {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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