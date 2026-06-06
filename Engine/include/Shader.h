#pragma once

#include "EngineCommon.h"

namespace Engine {
    class Shader {
    public:
        Shader(const std::string& vertexSrcPath, const std::string& fragmentSrcPath);
        ~Shader();

        void Bind();
        void UnBind();

    private:
        unsigned int id;
        std::string ReadFile(const std::string& filePath);

        unsigned int CompileShader(unsigned int type, const std::string &source);
        void CompileProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}
