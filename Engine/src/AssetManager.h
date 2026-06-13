#pragma once

#include "Texture.h"

namespace Engine {
    class AssetManager {
    public:
        static const Texture* AddTexture(const std::string& name, const std::string& path);
        static const Texture* GetTexture(const std::string& name);
        static void ClearTexture(const std::string& name);
        static void ClearTextureAll();
    private:
        inline static std::unordered_map<std::string, std::unique_ptr<Texture>> s_textures;
    };
} // Engine
