#include "AssetManager.h"

namespace Engine {
    const Texture* AssetManager::AddTexture(const std::string &name, const std::string &relativePath) {
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

    const Texture* AssetManager::GetTexture(const std::string &name) {
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
