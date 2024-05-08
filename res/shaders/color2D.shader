#vertex
#version 330 core

layout(location = 0) in vec3 inPosition;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
}

#fragment
#version 330 core

out vec4 FragColor; // Final color output

uniform vec3 color;

void main() {
    // Output the uniform color
    FragColor = vec4(color, 1); // Assuming a white line}
}