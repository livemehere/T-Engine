#pragma once

#include "EngineCommon.h"

class Texture {
public:
    unsigned int id;
    int width;
    int height;
    int channels;

    Texture(const std::string& filepath);
    Texture(unsigned char *rawData, int w, int h, int ch);
    ~Texture() = default;
    static const Texture& GetWhiteTexture();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

private:
    unsigned char* data = nullptr;
    void UploadTexture();
};
