#include "core.h"

#include "RenderTexture.h"
#include "Log.h"
#include <iostream>

RenderTexture::RenderTexture() 
    : framebufferID(0), renderbufferID(0), width(0), height(0) 
{
    texture = std::make_shared<Texture>();

    // Create framebuffer object (FBO)
    glGenFramebuffers(1, &framebufferID);
}

RenderTexture::~RenderTexture() {
    Cleanup();
}

bool RenderTexture::Init(int width, int height, GLuint attachment) {
    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // Create texture as render target
    texture->Init(width, height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->GetTextureID(), 0);
    // attach renderbuffer for depth testing
    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbufferID);

    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Console::Error("Framebuffer is not complete!");
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
}

void RenderTexture::Clear()
{
    glBindBuffer(GL_FRAMEBUFFER, framebufferID);
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
