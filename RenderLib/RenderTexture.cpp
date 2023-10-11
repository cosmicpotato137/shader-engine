#include "core.h"

#include "RenderTexture.h"
#include <iostream>

RenderTexture::RenderTexture(int width, int height) 
    : framebufferID(0), renderbufferID(0), width(width), height(height) 
{
    texture = std::make_shared<Texture>();
}

RenderTexture::~RenderTexture() {
    Cleanup();
}

bool RenderTexture::Init() {
    // Create framebuffer object (FBO)
    glGenFramebuffers(1, &framebufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // Create texture as render target
    texture->Init(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);

    // attach renderbuffer for depth testing
    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferID);

    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Framebuffer is not complete!" << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind FBO

    return true;
}

void RenderTexture::Cleanup()
{
    texture->Cleanup();
    glDeleteBuffers(1, &renderbufferID);
    glDeleteBuffers(1, &framebufferID);
}

void RenderTexture::BeginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTexture::EndRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::Bind()
{
    texture->Bind();
}

void RenderTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool RenderTexture::SaveToImage(const char* filePath)
{
    return texture->SaveToImage(filePath);
}

ptr<Texture> RenderTexture::GetTexture() const {
    return texture;
}
