#vertex
#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

#fragment
#version 430 core

in vec2 TexCoord;  // Texture coordinate of the current fragment
out vec4 FragColor;

uniform sampler2D renderTexture;  // The texture you want to sample

void main() {
    FragColor = texture(renderTexture, TexCoord);
}