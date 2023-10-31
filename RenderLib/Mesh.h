#pragma once
#include "core.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

class Mesh {
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    Mesh(const std::string& filePath) 
        : VAO(0), VBO(0), EBO(0) 
    {
        LoadMesh(filePath);
    }
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) 
        : vertices(vertices), indices(indices)
    { 
        SetupMesh(); 
    }

    ~Mesh();
    void Draw();

    static ptr<Mesh> Quad();
    static ptr<Mesh> Cube();
    static ptr<Mesh> Sphere(int rings, int segments);

private:
    void LoadMesh(const std::string& filePath);
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void SetupMesh();

};
