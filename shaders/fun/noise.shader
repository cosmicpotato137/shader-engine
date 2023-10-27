#vertex
#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord; // UV coordinates

out vec2 TexCoord; // Pass UV coordinates to the fragment shader

void main()
{
    gl_Position = vec4(inPosition, 1.0f);
    TexCoord = inTexCoord; // Pass UV coordinates to fragment shader
}

#fragment
#version 330 core

out vec4 FragColor;

// Simple random number generator
uint xorshift32(uint state) {
    state ^= (state << 13);
    state ^= (state >> 17);
    state ^= (state << 5);
    return state;
}

void main() {
    // Generate random colors using built-in GLSL functions
    // vec3 randomColor = vec3(fract(sin(time + gl_FragCoord.x * 12.9898) * 43758.5453),
    //                         fract(cos(time + gl_FragCoord.y * 34.1414) * 98765.4321),
    //                         fract(sin(time + gl_FragCoord.x * 12.9898) * cos(gl_FragCoord.y * 34.1414)));

    // Use gl_FragCoord as a seed value (you can use a uniform or other values as seed)
    uint seed = uint(gl_FragCoord.x) + uint(gl_FragCoord.y) * uint(1973);

    // Generate a random value between 0 and 1
    float randomValue = float(xorshift32(seed)) / float(0xFFFFFFFFu);

    // Threshold the value to black (0.0) or white (1.0)
    if (randomValue < 0.5)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Black
    else
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White
}
