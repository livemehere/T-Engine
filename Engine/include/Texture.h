#pragma once

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
        Texture(const std::string& filepath);
        Texture(unsigned char *rawData, int w, int h, int ch);
        ~Texture();

        static std::shared_ptr<Texture> GetWhiteTexture();

        unsigned int GetId() const { return m_id; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        int GetChannels() const { return m_channels; }

        void Bind(unsigned int slot = 0) const;
        void UnBind() const;

    private:
        unsigned int m_id = 0;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;
        unsigned char* m_data = nullptr;
        void UploadTexture();
    };
}
