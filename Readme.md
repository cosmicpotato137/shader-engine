# Shader Engine

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Building the Project](#building-the-project)
4. [Usage Examples](#usage-examples)
5. [Contributing](#contributing)

## Introduction

This project is a rendering pipeline for compiling GLSL shaders. It provides a lightweight framework for loading, editing and serializing compute shaders with options for user interaction. Currently, Shader Engine is designed for experimenting learning with shaders, but it can also be modified to support materials systems and mesh rendering.

Shader Engine only supports windows for now, I do have vague plans to get it running on linux. Until then, PRs are welcome!

## Features

- ImGui interface
  - Shader info windows
  - Error console
  - Scene view
  - File dialog
- Realtime shader uniform updates
- Uniform serialization
- Event system for handling user input

## Building the Project

To build this project you can run the following commands:

1. Clone the repository and submodules

```bash
git clone --recurse-submodules https://github.com/cosmicpotato137/shader-engine.git
```

OR

```bash
git clone https://github.com/cosmicpotato137/shader-engine.git
git submodule init
git submodule update
```

2. Build the project with CMake

```bash
mkdir build
cd build
cmake ..
```

3. Compile with VSCode or Visual studio. The config files for VSCode are included, and the Visual Studio solution will be in `./build`.

## Examples

Most changes that you might want to make to this project before you build are likely either in these files:

- `RenderEngine/RenderLayer.cpp`:
  This file contains an example of Conway's Game of Life, with scalable pixel size and a brush to create your own automata.
- `RenderEngine/SimulationLayer.cpp`
  This file contains a particle system that is run using either a boid or a slime shader to update the agent positions.
- `RenderEngine/main.cpp`
  There is no way to switch the examples at runtime (yet) but layers can be added or removed in main by commenting any of these lines:

```cpp
// app->PushLayer(std::make_shared<RenderLayer>()); // User input
app->PushLayer(std::make_shared<SimulationLayer>()); // Particle systems
app->PushLayer(std::make_shared<OutputLayer>()); // Shader output console
```

## Contributing

There are a number of things I would love to add to this project, when I have time, but until then, I would welcome PRs on any of these topics (from most to least important):

- Linux compatibility
- Model Importing with assimp
- Material system
- Better particle system
