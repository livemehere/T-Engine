#include "Shader.h"
#include <fstream>

namespace Engine {
    Shader::Shader(const std::string &vertexSrcPath, const std::string &fragmentSrcPath) {
        std::string vertexSrc = ReadFile(vertexSrcPath);
        std::string fragmentSrc = ReadFile(fragmentSrcPath);
        CompileProgram(vertexSrc, fragmentSrc);
    }

    Shader::~Shader() {
        glDeleteProgram(id);
    }

    void Shader::Bind() {
        glUseProgram(id);
    }

    void Shader::UnBind() {
        glUseProgram(0);
    }

    std::string Shader::ReadFile(const std::string &filepath) {
        std::string result;
        std::ifstream in(filepath);
        if (in) {
            std::stringstream ss;
            ss << in.rdbuf();
            result = ss.str();
        } else {
            LOG_ERROR("[Shader] Failed to read file {}", filepath);
        }
        return result;
    }

    unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
        std::string typeStr = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
        const char* sourcePtr = source.c_str();

        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(success, 512, nullptr, infoLog);
            LOG_ERROR("[Shader] {} compile error \n {}", typeStr, infoLog);
        }

        return shader;
    }

    void Shader::CompileProgram(const std::string &vertexSrc, const std::string &fragmentSrc) {
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        int success;
        char infoLog[512];

        id = glCreateProgram();

        glAttachShader(id, vs);
        glAttachShader(id, fs);
        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(id, 512, nullptr, infoLog);
            LOG_ERROR("[Shader] Failed to compile program\n {}", infoLog);
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }
}
