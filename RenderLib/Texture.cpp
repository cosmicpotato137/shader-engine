#include "Texture.h"

#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>


Texture::Texture() : textureID(0), width(0), height(0) {
    // Generate and bind the texture
    glGenTextures(1, &textureID);
}

bool Texture::Init(const std::string& filename)
{
    // Generate and bind the texture
    glGenTextures(1, &textureID);
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters (you can customize these)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and set the image data to the texture
    // You can use a library like SOIL or stb_image to load images
    // Here, we assume you have loaded the image into a buffer

    // using stb_image
    int channels;
    unsigned char* image = stbi_load(filename.c_str(), &width, &height, &channels, 4);
    if (!image) {
        Console::Error("Failed to load texture image: %s", filename.c_str());
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image data after loading
    stbi_image_free(image);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

bool Texture::Init(int width, int height)
{    
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters (you can customize these)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set a texture blank texture
    char* image = (char*)malloc(width * height * 4 * sizeof(char));
    for (int i = 0; i < width * height * 4; i++)
        image[i] = 0;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    free(image);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::BindCompute(int binding) const
{
    glBindImageTexture(binding, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

bool Texture::SaveToImage(const std::string& filePath)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    unsigned char* pixelData = new unsigned char[4 * width * height];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    // Reverse the rows in pixelData to fix the orientation
    int rowSize = 4 * width;
    unsigned char* tempRow = new unsigned char[rowSize];
    for (int i = 0; i < height / 2; i++) {
        int rowIndex1 = i * rowSize;
        int rowIndex2 = (height - 1 - i) * rowSize;
        memcpy(tempRow, pixelData + rowIndex1, rowSize);
        memcpy(pixelData + rowIndex1, pixelData + rowIndex2, rowSize);
        memcpy(pixelData + rowIndex2, tempRow, rowSize);
    }
    delete[] tempRow;

    // Save the corrected pixelData to an image file using stb_image_write
    if (stbi_write_png(filePath.c_str(), width, height, 4, pixelData, width * 4) != 0) {
        Console::Log("RenderTexture saved to: %s", filePath.c_str());
    }
    else {
        Console::Error("Failed to save RenderTexture to an image file");
    }

    delete[] pixelData;
    return true; // Replace with proper error handling
}


void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Cleanup()
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}
