#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord; // UV coordinates

out vec2 texCoord; // Pass UV coordinates to the fragment shader
out float texTime;

uniform mat4 model;
uniform mat4 viewProjection;
uniform float time;

void main()
{
    float amplitude = .2;
    float frequency = 10;
    float offset = 0;
    vec4 oldpos = vec4(inPosition, 1.0);
    oldpos.x += oldpos.x * (sin(time + oldpos.y * frequency) + offset) * amplitude;
    oldpos.z += oldpos.z * (sin(time + oldpos.y * frequency) + offset) * amplitude;
    gl_Position = viewProjection * model * oldpos;
    texCoord = inTexCoord; // Pass UV coordinates to fragment shader
    texTime = time / 35;
}
