#include "Texture.h"
#include "../lib/stb_image.h"

namespace Engine {
    Texture::Texture(const std::string &filepath) {
        LOG_INFO("[Texture] ctor from file {}", filepath);
        stbi_set_flip_vertically_on_load(true);
        m_data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, STBI_rgb_alpha);
        if (!m_data) {
            throw std::runtime_error(std::string("[Texture] image load failed: ") + filepath);
        }
        LOG_INFO("[Texture] image loaded {}x{} (channels: {})", m_width, m_height, m_channels);
        UploadTexture();
        stbi_image_free(m_data);
        m_data = nullptr;
    }

    Texture::Texture(unsigned char *rawData, int w, int h, int ch)
        : m_id(0), m_width(w), m_height(h), m_channels(ch), m_data(rawData) {
        if (m_data == nullptr) {
            throw std::runtime_error("[Texture] raw texture data cannot be null");
        }

        if (m_width <= 0 || m_height <= 0) {
            throw std::runtime_error("[Texture] texture dimensions must be greater than zero");
        }

        LOG_INFO("[Texture] ctor from raw data {}x{} (channels: {})", m_width, m_height, m_channels);
        UploadTexture();
    }

    Texture::~Texture() {
        LOG_INFO("[Texture] dtor (id: {})", m_id);
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
            LOG_INFO("[Texture] VRAM data free (id: {})", m_id);
            m_id = 0;
        }
    }

    void Texture::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0+slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::UnBind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::UploadTexture() {
        if (m_data == nullptr) {
            throw std::runtime_error("[Texture] cannot upload null texture data");
        }

        if (m_width <= 0 || m_height <= 0) {
            throw std::runtime_error("[Texture] cannot upload texture with invalid dimensions");
        }

        glGenTextures(1, &m_id);
        if (m_id == 0) {
            throw std::runtime_error("[Texture] failed to create texture object");
        }

        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
        LOG_INFO("[Texture] GPU upload success (id: {})", m_id);
    }
}
