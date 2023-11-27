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

    bool SaveToImage(const char* filePath);

    ptr<Texture> GetTexture() const;

private:
};

