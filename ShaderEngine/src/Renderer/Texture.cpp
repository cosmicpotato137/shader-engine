#include "Texture.h"

#include "Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Texture::Texture() : textureID(0), width(0), height(0), image(nullptr) {
  // Generate the texture
  glGenTextures(1, &textureID);
}

bool Texture::Init(const std::string &filename) {
  this->filename = filename;

  // Generate and bind the texture
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

  // Using stb_image
  int channels;
  image = (char *)stbi_load(filename.c_str(), &width, &height, &channels, 4);
  if (!image) {
    Console::Error("Failed to load texture image: %s", filename.c_str());
    return false;
  }

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA32F,
      width,
      height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      image);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}

bool Texture::Init(int width, int height) {
  if (width <= 0 || height <= 0) {
    Console::Error("Invalid texture dimensions: %d x %d", width, height);
    this->width = 0;
    this->height = 0;
    return false;
  }

  this->width = width;
  this->height = height;

  glBindTexture(GL_TEXTURE_2D, textureID);

  // Set texture parameters (you can customize these)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set a texture blank texture
  image = (char *)malloc(width * height * 4 * sizeof(char));
  for (int i = 0; i < width * height * 4; i++)
    image[i] = 0;

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA32F,
      width,
      height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      image);

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  return true;
}

void Texture::WritePixel(
    int x, int y, unsigned char r, unsigned char g, unsigned char b,
    unsigned char a) {
  if (!image || x < 0 || x >= width || y < 0 || y >= height) {
    Console::Error("Invalid pixel coordinates: (%d, %d)", x, y);
    Console::Assert(width > 0 && height > 0, "Texture not initialized");
    return;
  }

  int index = (y * width + x) * 4;  // Assuming RGBA format
  image[index] = r;
  image[index + 1] = g;
  image[index + 2] = b;
  image[index + 3] = a;

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexSubImage2D(
      GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, image + index);
  glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec4 Texture::GetPixel(int x, int y) {
  int index = (y * width + x) * 4;  // Assuming RGBA format

  // get values from texture
  GetImageData();

  unsigned char r = image[index];
  unsigned char g = image[index + 1];
  unsigned char b = image[index + 2];
  unsigned char a = image[index + 3];

  return glm::vec4(r, g, b, a);
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, textureID); }

void Texture::BindCompute(int binding) const {
  glBindImageTexture(
      binding, textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

bool Texture::SaveToImage(const std::string &filePath) {
  GetImageData();

  // Reverse the rows in pixelData to fix the orientation
  int rowSize = 4 * width;
  unsigned char *tempRow = new unsigned char[rowSize];
  for (int i = 0; i < height / 2; i++) {
    int rowIndex1 = i * rowSize;
    int rowIndex2 = (height - 1 - i) * rowSize;
    memcpy(tempRow, image + rowIndex1, rowSize);
    memcpy(image + rowIndex1, image + rowIndex2, rowSize);
    memcpy(image + rowIndex2, tempRow, rowSize);
  }
  delete[] tempRow;

  // Save the corrected image to an image file using stb_image_write
  if (stbi_write_png(filePath.c_str(), width, height, 4, image, width * 4) !=
      0) {
    Console::Log("RenderTexture saved to: %s", filePath.c_str());
  } else {
    Console::Error("Failed to save RenderTexture to an image file");
  }

  return true;  // Replace with proper error handling
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::Cleanup() {
  if (textureID != 0) {
    glDeleteTextures(1, &textureID);
    textureID = 0;
  }
  if (image != nullptr) {
    free(image);
    image = nullptr;
  }
}

void Texture::GetImageData() {
  glBindTexture(GL_TEXTURE_2D, textureID);
  glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
}

std::ostream &operator<<(std::ostream &os, const Texture &texture) {
  os << texture.filename << '\n';
  os << texture.width << '\n';
  os << texture.height << '\n';

  return os;
}

std::istream &operator>>(std::istream &is, Texture &texture) {
  // Read object data
  getline(is, texture.filename);
  is >> texture.width;
  is >> texture.height;

  // Initialize the texture with the filepath
  if (texture.filename != "") {
    texture.Init(texture.filename);
  } else {
    texture.Init(texture.width, texture.height);
  }

  return is;
}
