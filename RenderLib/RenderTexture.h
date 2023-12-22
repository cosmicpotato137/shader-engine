#pragma once

#include "core.h"
#include "Texture.h"

class RenderTexture {
    ptr<Texture> texture;
    GLuint renderbufferID;
    GLuint framebufferID;
    int width;
    int height;

public:
    RenderTexture();
    ~RenderTexture();

    bool Init(int width, int height, bool attachDepthStencil);
    void Cleanup();
    void BeginRender(bool clear = true);
    void EndRender();
    void Bind();
    void Unbind();
    void Clear();

    int GetWidth() { return texture->GetWidth(); }
    int GetHeight() { return texture->GetHeight(); }
    glm::vec2 GetSize() { return texture->GetSize(); }

    bool SaveToImage(const std::string& filePath);

    ptr<Texture> GetTexture() const;

private:
};

