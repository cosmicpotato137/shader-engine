#pragma once
#include "Mesh.h"

#include "Log.h"

Mesh::~Mesh()
{
    Cleanup();
}

void Mesh::Draw()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Cleanup()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::LoadMesh(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
    SetupMesh();
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    for (GLuint i = 0; i < node->mNumChildren; ++i) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        // You can also add code here to process other attributes like normals and texture coordinates
        //vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        vertices.push_back(vertex);
    } 

    for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Bind and set up the vertex buffer for positions and UVs
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    //                 index, number of, type, norm?, bytes between, starting offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // UV attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}


ptr<Mesh> Mesh::Quad()
{
    // Vertex data for a square
    std::vector<Vertex> vertices = {
        { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } }, // Bottom-left
        { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } }, // Bottom-right
        { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } }, // Top-right
        { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } }  // Top-left
    };

    // Index data for the square
    std::vector<GLuint> indices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    std::vector<GLfloat> uvs = {

    };

    return std::make_shared<Mesh>(vertices, indices);
}

ptr<Mesh> Mesh::Cube() {
    std::vector<Vertex> vertices = {
        // Front face (reversed)
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},

        // Back face (reversed)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},

        // Left face (reversed)
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},

        // Right face (reversed)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},

        // Top face (reversed)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},

        // Bottom face (reversed)
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}
    };

    std::vector<unsigned int> indices = {
        // Front face (reversed)
        0, 2, 1,
        2, 0, 3,

        // Back face (reversed)
        4, 5, 6,
        6, 7, 4,

        // Left face (reversed)
        8, 9, 10,
        10, 11, 8,

        // Right face (reversed)
        12, 14, 13,
        14, 12, 15,

        // Top face (reversed)
        16, 18, 17,
        18, 16, 19,

        // Bottom face (reversed)
        20, 21, 22,
        22, 23, 20
    };


    return std::make_shared<Mesh>(vertices, indices);
}

ptr<Mesh> Mesh::Sphere(int rings, int segments)
{
    // Function to create a vertex and index buffer for a sphere
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= rings; ++i) {
        float phi = i * glm::pi<float>() / rings;
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        for (int j = 0; j <= segments; ++j) {
            float theta = j * 2 * glm::pi<float>() / segments;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            float x = cosTheta * sinPhi;
            float y = cosPhi;
            float z = sinTheta * sinPhi;

            float u = static_cast<float>(j) / segments;
            float v = static_cast<float>(i) / rings;

            vertices.push_back({ {x, y, z}, {u, v} });
        }
    }

    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = i * (segments + 1) + j;
            int second = first + segments + 1;

            indices.push_back(first);
            indices.push_back(first + 1);
            indices.push_back(second);

            indices.push_back(second);
            indices.push_back(first + 1);
            indices.push_back(second + 1);
        }
    }

    return std::make_shared<Mesh>(vertices, indices);
}