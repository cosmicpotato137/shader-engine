#vertex
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


#fragment
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
