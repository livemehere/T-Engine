#include "AssetManager.h"

namespace Engine {
     Texture* AssetManager::AddTexture(const std::string &name, const std::string &relativePath) {
        if (const auto found = GetTexture(name)) {
           return found;
        }
        auto texture = std::make_unique<Texture>();
        auto path = ResolvePath(relativePath);
        if (!texture->LoadFromFile(path)) {
           return nullptr;
        }
        s_textures[name] = std::move(texture);
        return s_textures[name].get();
    }

    Texture* AssetManager::GetTexture(const std::string &name) {
        if (const auto it = s_textures.find(name); it != s_textures.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void AssetManager::ClearTexture(const std::string &name) {
        s_textures.erase(name);
    }

    void AssetManager::ClearTextureAll() {
        s_textures.clear();
    }

    Shader * AssetManager::AddShader(const std::string &name, const std::string &vertexShaderPath,
        const std::string &fragShaderPath) {
        if (const auto found = GetShader(name)) {
            return found;
        }
        // shader throw error if load failed
        auto shader = std::make_unique<Shader>(ResolvePath(vertexShaderPath), ResolvePath(fragShaderPath));
        s_shaders[name] = std::move(shader);
        return s_shaders[name].get();
    }

    Shader * AssetManager::GetShader(const std::string &name) {
        if (const auto it = s_shaders.find(name); it != s_shaders.end()) {
            return (*it).second.get();
        }
        return nullptr;
    }

    void AssetManager::ClearShader(const std::string &name) {
        s_shaders.erase(name);
    }

    void AssetManager::ClearShaderAll() {
        s_shaders.clear();
    }

    void AssetManager::SetAssetRoot(const std::filesystem::path &rootPath) {
        s_rootPath = rootPath;
    }

    std::filesystem::path AssetManager::ResolvePath(const std::filesystem::path &relativePath) {
        if (relativePath.is_absolute()) {
            return relativePath;
        }

        return s_rootPath / relativePath;
    }
}
