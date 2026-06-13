#pragma once

namespace Engine {
    class Shader {
    public:
        Shader(const std::string& vertexSrcPath, const std::string& fragmentSrcPath);
        ~Shader();

        unsigned int GetId() const { return m_id; }
        void Bind();
        void UnBind();

    private:
        unsigned int m_id = 0;
        std::string ReadFile(const std::string& filePath);

        unsigned int CompileShader(unsigned int type, const std::string &source);
        void CompileProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
