#version 330 core

// input and output
in vec2 TexCoord;
out vec4 FragColor;

// The input texture sampler
uniform sampler2D renderTexture;

uniform float time;


void main() {
    // Sample the color from the input render texture
    vec4 color = texture(renderTexture, TexCoord);

    // Output the sampled color as the final color
    float t = time;
    FragColor = color * vec4((sin(t) + 1) / 2, (cos(t) + 1) / 2, 0.0, 1.0);

    // Output texCoord as color
    //FragColor = vec4(1, 0, 0.0f, 1.0f);
}
