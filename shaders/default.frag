#version 330 core

in vec2 fragTexCoord; // Input color from vertex shader

out vec4 fragOutput; // Final color output

void main() {
    // Output the uniform color
    fragOutput = vec4(1, 0, 1, 1);
}
