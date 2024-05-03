#pragma once
#include "Core/core.h"

#include <iostream>
#include <string>

class Texture {
  GLuint textureID;
  int width;
  int height;

  std::string filename;

  char *image;

public:
  Texture();
  ~Texture() { Cleanup(); }

  // Initialize the texture from an image file
  bool Init(const std::string &filename);

  // Initialize the texture to black
  bool Init(int width, int height);

  // Set the pixel at (x, y) to the given color
  void WritePixel(
      int x, int y, unsigned char r, unsigned char g, unsigned char b,
      unsigned char a = 255);

  // Get the pixel at (x, y)
  glm::vec4 GetPixel(int x, int y);

  // Bind the texture for rendering
  void Bind() const;

  // Bind the texture for compute rendering
  void BindCompute(int binding) const;

  // Save texture to image
  bool SaveToImage(const std::string &filePath);

  // Unbind the currently bound texture
  void Unbind() const;

  // Get the texture width
  int GetWidth() const { return width; }

  // Get the texture height
  int GetHeight() const { return height; }

  glm::vec2 GetSize() const { return glm::vec2(width, height); }

  // Get the texture ID
  int GetTextureID() { return textureID; }

  // Clean up and release resources
  void Cleanup();

  // Serialization
  friend std::ostream &operator<<(std::ostream &os, const Texture &texture);

  // Deserialization
  friend std::istream &operator>>(std::istream &is, Texture &texture);

private:
  void GetImageData();
};
