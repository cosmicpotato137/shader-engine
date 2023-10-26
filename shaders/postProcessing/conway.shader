#vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

#fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform float time;
uniform int step;
uniform sampler2D currentTexture;
uniform ivec2 textureSize;
uniform ivec2 mousePosition; // New uniform to pass the mouse position
uniform bool addCell; // New uniform to check if the user is adding cells

vec4 conway()
{
    ivec2 cell = ivec2(TexCoord * textureSize);
    ivec2 neighbors[8] = ivec2[8](
        ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1),
        ivec2(-1, 0),                  ivec2(1, 0),
        ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1)
    );
    
    int liveNeighbors = 0;
    

    
    for (int i = 0; i < 8; i++) {
        ivec2 neighbor = cell + neighbors[i];
        if (neighbor.x >= 0 && neighbor.x < textureSize.x &&
            neighbor.y >= 0 && neighbor.y < textureSize.y) {
            if (texture(currentTexture, (neighbor + 0.5) / textureSize).r > 0.5)
                liveNeighbors++;
        }
    }
    
    bool isAlive = texture(currentTexture, (cell + 0.5) / textureSize).r > 0.5;
    
    if (isAlive) {
        // Any live cell with fewer than two live neighbors dies (underpopulation)
        // Any live cell with two or three live neighbors lives on to the next generation
        // Any live cell with more than three live neighbors dies (overpopulation)
        if (liveNeighbors < 2 || liveNeighbors > 3)
            return vec4(0.0, 0.0, 0.0, 1.0); // Dead
        else
            return vec4(1.0, 1.0, 1.0, 1.0); // Alive
    } else {
        // Any dead cell with exactly three live neighbors becomes a live cell (reproduction)
        if (liveNeighbors == 3)
            return vec4(1.0, 1.0, 1.0, 1.0); // Alive
        else
            return vec4(0.0, 0.0, 0.0, 1.0); // Dead
    }
}

void main() {

    if (step == 1)
    {
        FragColor = conway();
    }
    else
    {
        FragColor = texture(currentTexture, TexCoord);
    }

    FragColor = vec4(1, sin(time), 0, 1);
    
    // Check if the current pixel corresponds to the mouse position
    // if (addCell && cell == mousePosition) {
    //     FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Add a cell at the mouse position
    //     return;
    // }
}
