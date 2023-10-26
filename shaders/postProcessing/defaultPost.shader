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

uniform float time;
uniform sampler2D renderTexture;  // The texture you want to sample


// void main() {
//     vec2 texelSize = 1.0 / textureSize(renderTexture, 0); // Calculate the size of a texel

//     // Sample the current fragment's texel
//     vec4 currentPixel = texture(renderTexture, TexCoord);

//     // Sample neighboring pixels
//     vec4 leftPixel = texture(renderTexture, TexCoord - vec2(texelSize.x, 0));
//     vec4 rightPixel = texture(renderTexture, TexCoord + vec2(texelSize.x, 0));
//     vec4 topPixel = texture(renderTexture, TexCoord + vec2(0, texelSize.y));
//     vec4 bottomPixel = texture(renderTexture, TexCoord - vec2(0, texelSize.y));

//     // Now you can work with these neighboring pixels, e.g., calculate gradients, apply filters, etc.
    
//     // For example, compute the average color of surrounding pixels:
//     vec4 averageColor = (currentPixel + leftPixel + rightPixel + topPixel + bottomPixel) / 5.0;

//     FragColor = averageColor;
// }

void main() {
    // Generate random colors using built-in GLSL functions
    // vec3 randomColor = vec3(fract(sin(time + gl_FragCoord.x * 12.9898) * 43758.5453),
    //                         fract(cos(time + gl_FragCoord.y * 34.1414) * 98765.4321),
    //                         fract(sin(time + gl_FragCoord.x * 12.9898) * cos(gl_FragCoord.y * 34.1414)));

    FragColor = texture(renderTexture, TexCoord);
}