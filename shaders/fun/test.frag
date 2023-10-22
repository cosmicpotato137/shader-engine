#version 330 core

in vec2 texCoord; // UV coordinates from vertex shader
in float texTime;

out vec4 fragColor;

uniform sampler2D textureSampler; // Texture sampler

void main()
{
    // Sample the texture using UV coordinates
    vec4 textureColor = texture(textureSampler, texCoord + vec2(0, -texTime));
    fragColor = textureColor;
}
