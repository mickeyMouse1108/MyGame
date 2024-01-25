﻿#include "Texture.h"
#include "Debugging.h"
#include "GraphicsDevice.h"
#include "../vendor/stb_image/stb_image.h"

namespace Graphics {
    Texture::Texture(const uchar* raw, uint w, uint h, bool useLinear, int format, int alignment)
        : rendererID(0), size { w, h } {
        //flips texture
        LoadTexture(raw, useLinear, format, alignment);
    }

    Texture::Texture(const std::string& filePath, bool useLinear)
        : rendererID(0) {
        //flips texture
        stbi_set_flip_vertically_on_load(1);
        const uchar* localTexture = stbi_load(filePath.c_str(), (int*)&size.x, (int*)&size.y, &BPPixel, 4);
        LoadTexture(localTexture, useLinear);
        if (localTexture) stbi_image_free((void*)localTexture);
    }

    Texture::~Texture() {
        Destroy();
    }

    void Texture::LoadTexture(const uchar* img, bool useLinear, int format, int alignment) {
        GLCALL(glGenTextures(1, &rendererID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, alignment));
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, img));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture::Transfer(Texture& dest, Texture&& from) {
        dest.rendererID = from.rendererID;
        from.rendererID = 0;

        dest.size = from.size;
        dest.BPPixel = from.BPPixel;
    }

    Texture Texture::LoadPNGBytes(const uchar* png, int len, bool useLinear) {
        Texture tex;
        
        stbi_set_flip_vertically_on_load(1);
        const uchar* localTexture = stbi_load_from_memory(png, len, (int*)&tex.size.x, (int*)&tex.size.y, &tex.BPPixel, 4);
        tex.LoadTexture(localTexture, useLinear);
        if (localTexture) stbi_image_free((void*)localTexture);
        
        return tex;
    }

    void Texture::Bind(uint slot, GraphicsDevice* gdevice) {
        device = gdevice;
        textureSlot = slot;
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }

    void Texture::Unbind() {
        if (!rendererID) return;
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
        if (device)
            device->textures[textureSlot] = nullptr;
        textureSlot = 0;
        device = nullptr;
    }

    void Texture::Destroy() {
        Unbind();
        if (rendererID == GL_NULL) return;
        GLCALL(glDeleteTextures(1, &rendererID));
        rendererID = GL_NULL;
    }
}
