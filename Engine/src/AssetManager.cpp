#include "AssetManager.h"

namespace Engine {
    std::unordered_map<std::string, std::shared_ptr<Texture>> AssetManager::s_textures;

    std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string &name, const std::string &path) {
        if (IsTextureExists(name)) {
            return s_textures[name];
        }
        auto texture = std::make_shared<Texture>(path);
        s_textures[name] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::LoadTexture(const std::string &path) {
        if (IsTextureExists(path)) {
            return s_textures[path];
        }
        auto texture = std::make_shared<Texture>(path);
        s_textures[path] = texture;
        return texture;
    }

    std::shared_ptr<Texture> AssetManager::GetTexture(const std::string &name) {
        if (IsTextureExists(name)) {
            return s_textures[name];
        }
        return nullptr;
    }

    bool AssetManager::IsTextureExists(const std::string &name) {
        return s_textures.contains(name);
    }

    void AssetManager::Shutdown() {
        s_textures.clear();
    }
} // Engine
