#vertex
#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord; // UV coordinates

out vec2 TexCoord; // Pass UV coordinates to the fragment shader

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * model * vec4(inPosition, 1.0);
    TexCoord = inTexCoord; // Pass UV coordinates to fragment shader
}

#fragment
#version 330 core

in vec2 TexCoord; // Input color from vertex shader

out vec4 fragOutput; // Final color output

void main() {
    // Output the uniform color
    fragOutput = vec4(1, 0, 1, 1);
}
