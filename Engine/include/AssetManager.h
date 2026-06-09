#pragma once

#include "Texture.h"

namespace Engine {
    class AssetManager {
    public:
        static std::shared_ptr<Texture> LoadTexture(const std::string& name, const std::string& path);
        static std::shared_ptr<Texture> LoadTexture(const std::string& path);
        static std::shared_ptr<Texture> GetTexture(const std::string& name);
        static bool IsTextureExists(const std::string& name);
        static void Shutdown();
    private:
        static std::unordered_map<std::string, std::shared_ptr<Texture>> s_textures;
    };
} // Engine
