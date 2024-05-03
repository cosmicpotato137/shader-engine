#pragma once
#include "Core/core.h"
#include "Core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec2 TexCoords;
};

class Mesh {
  GLuint VAO, VBO, EBO;
  std::vector<Vertex> vertices;
  std::vector<int> indices;

public:
  // Load mesh from file
  Mesh(const std::string &filePath);

  // Load mesh from vertices and indices
  Mesh(std::vector<Vertex> vertices, std::vector<int> indices);

  ~Mesh();

  // Draw mesh (calls glDrawElements)
  void Draw(GLuint geometryType = GL_TRIANGLES);

  // Get quad mesh
  static ptr<Mesh> Quad();

  // Get cube mesh
  static ptr<Mesh> Cube();

  // Get sphere mesh from the number of rings and segments
  static ptr<Mesh> Sphere(int rings, int segments);

  friend std::ostream &operator<<(std::ostream &os, const Mesh &m) {
    Console::Assert(false, "Not implemented");
  }
  friend std::istream &operator>>(std::istream &is, Mesh &m) {
    Console::Assert(false, "Not implemented");
  }

private:
  // Load mesh from file
  void LoadMesh(const std::string &filePath);

  void ProcessNode(aiNode *node, const aiScene *scene);
  void ProcessMesh(aiMesh *mesh, const aiScene *scene);

  void SetGeometryBuffers(
      std::vector<Vertex> vertices, std::vector<int> indices, GLuint &vao,
      GLuint &vbo, GLuint &ebo);
};
