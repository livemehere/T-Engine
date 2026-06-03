#include "Texture.h"
#include "../lib/stb_image.h"

Texture::Texture(const std::string &filepath) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filepath.c_str(),&width, &height,&channels, STBI_rgb_alpha);
    if (!data) {
        LOG_ERROR("[Texture] image load failed {}", filepath);
        return;
    }
    LOG_INFO("[Texture] image loaded {}x{} (channels: {})", width, height, channels);
    UploadTexture();
    stbi_image_free(data);
    data = nullptr;
}

Texture::Texture(unsigned char *rawData, int w, int h, int ch) : id(0), data(rawData), width(w), height(h), channels(ch) {
    UploadTexture();
}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
        LOG_INFO("[Texture] VRAM data free (id: {})", id);
        id = 0;
    }
}

const Texture & Texture::GetWhiteTexture() {
    static unsigned char whitePixel[] = { 255, 255, 255, 255 };
    static Texture whiteTexture{whitePixel, 1,1,4};
    return whiteTexture;
}

void Texture::Bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::UnBind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UploadTexture() {
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE, data);
    LOG_INFO("[Texture] GPU upload success (id: {})", id);
}

