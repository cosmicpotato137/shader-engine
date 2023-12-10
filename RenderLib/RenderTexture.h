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

    bool Init(int width, int height, GLuint attachment = GL_COLOR_ATTACHMENT0);
    void Cleanup();
    void BeginRender();
    void EndRender();
    void Bind();
    void Unbind();
    void Clear();

    int GetWidth() { return texture->GetWidth(); }
    int GetHeight() { return texture->GetHeight(); }
    glm::vec2 GetSize() { return texture->GetSize(); }

    bool SaveToImage(const char* filePath);

    ptr<Texture> GetTexture() const;

private:
};

