#pragma once

namespace Engine {
    class Shader {
    public:
        Shader(const std::string& vertexSrcPath, const std::string& fragmentSrcPath);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        GLuint GetId() const { return m_id; }
        void Bind() const;
        void UnBind();

    private:
        GLuint m_id = 0;

        static std::string ReadFile(const std::string& filePath);
        static GLuint CompileShader(GLuint type, const std::string &source);

        void CompileProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
