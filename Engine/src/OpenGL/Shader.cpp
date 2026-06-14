#include "Shader.h"
#include <fstream>

namespace Engine {
    Shader::Shader(const std::string &vertexSrcPath, const std::string &fragmentSrcPath) {
        std::string vertexSrc = ReadFile(vertexSrcPath);
        std::string fragmentSrc = ReadFile(fragmentSrcPath);
        CompileProgram(vertexSrc, fragmentSrc);
    }

    Shader::~Shader() {
        glDeleteProgram(m_id);
    }

    void Shader::Bind() const {
        glUseProgram(m_id);
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
            throw std::runtime_error(std::string("[Shader] Failed to read file: ") + filepath);
        }

        if (result.empty()) {
            throw std::runtime_error(std::string("[Shader] Shader source is empty: ") + filepath);
        }

        return result;
    }

    GLuint Shader::CompileShader(GLuint type, const std::string &source) {
        std::string typeStr = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
        const char* sourcePtr = source.c_str();

        GLuint shader = glCreateShader(type);
        if (shader == 0) {
            throw std::runtime_error("[Shader] Failed to create " + typeStr + " shader object");
        }

        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            glDeleteShader(shader);
            throw std::runtime_error("[Shader] " + typeStr + " compile error\n" + std::string(infoLog));
        }

        return shader;
    }

    void Shader::CompileProgram(const std::string &vertexSrc, const std::string &fragmentSrc) {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        int success;
        char infoLog[512];

        m_id = glCreateProgram();
        if (m_id == 0) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            throw std::runtime_error("[Shader] Failed to create shader program");
        }

        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
            glDeleteProgram(m_id);
            m_id = 0;
            glDeleteShader(vs);
            glDeleteShader(fs);
            throw std::runtime_error(std::string("[Shader] Failed to compile program\n") + infoLog);
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }
}
