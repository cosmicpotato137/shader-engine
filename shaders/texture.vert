#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord; // UV coordinates

out vec2 fragTexCoord; // Pass UV coordinates to the fragment shader

uniform mat4 model;
uniform mat4 viewProjection;
uniform float time;

void main()
{
    gl_Position = viewProjection * model * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord; // Pass UV coordinates to fragment shader
}
