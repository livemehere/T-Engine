#pragma once

#include <filesystem>
#include <unordered_map>
#include "Texture.h"
#include "OpenGL/Shader.h"

namespace Engine {
    class AssetManager {
    public:
        static const Texture* AddTexture(const std::string& name, const std::string& relativePath);
        static const Texture* GetTexture(const std::string& name);
        static void ClearTexture(const std::string& name);
        static void ClearTextureAll();

        static const Shader* AddShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragShaderPath);
        static const Shader* GetShader(const std::string& name);
        static void ClearShader(const std::string& name);
        static void ClearShaderAll();

        static std::filesystem::path GetAssetRoot() { return s_rootPath; }
        static void SetAssetRoot(const std::filesystem::path &rootPath);

        static std::filesystem::path ResolvePath(const std::filesystem::path& relativePath);
    private:
        inline static std::unordered_map<std::string, std::unique_ptr<Texture>> s_textures;
        inline static std::unordered_map<std::string, std::unique_ptr<Shader>> s_shaders;
        inline static std::filesystem::path s_rootPath;
    };
}
