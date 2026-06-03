#pragma once

#include "EngineCommon.h"

/**
 * ⚠️ TODO: must managed by like 'AssetManager' with caching.
 * 1. Load data at RAM from file
 * 2. Upload data to V-RAM and delete RAM data
 * 3. Hold GPU Texture ID
 * 4. Delete V-RAM resource on destructor
 */
namespace Engine {
    class Texture {
    public:
        unsigned int id = 0;
        int width = 0;
        int height = 0;
        int channels = 0;

        Texture(const std::string& filepath);
        Texture(unsigned char *rawData, int w, int h, int ch);
        ~Texture();

        static std::shared_ptr<Texture> GetWhiteTexture();

        void Bind(unsigned int slot = 0) const;
        void UnBind() const;

    private:
        unsigned char* data = nullptr;
        void UploadTexture();
    };
}
