#pragma once
#include "core.h"
#include "Shader.h"

class ComputeShader : public Shader {
    GLuint computeShaderID;
    int x, y, z;
    GLuint memBarrier;

public:
    ComputeShader(const std::string& name) 
        : Shader(name), computeShaderID(0), x(1), y(1), z(1), memBarrier(GL_ALL_BARRIER_BITS)
    {}

    void SetWorkGroups(int numGroupsX, int numGroupsY, int numGroupsZ);
    void SetMemoryBarrier(GLuint memBarrier) { this->memBarrier = memBarrier; }

    bool InitFromSource(const std::string& computeSource) override;

    void Use() override;
};
