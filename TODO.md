# Todo list for shader-ideas

## RenderLib

- [x] testing classes for everything
- [x] Get testing project working with vscode gui
- [x] Look at warnings
- [x] weird glfw error on shutdown

## RenderEngine

- [ ] file dialogue for shaders
- [ ] scene imgui window class to abstract some layer code
- [x] generic scene object class
- [x] look at warnings
- [x] create log window/layer
- [x] serialization THIS IS COMPLICATED:
  - [x] need each instance of each object to be serialized separately
  - [x] objects that use other resources (shaders and textures) need those resources to be loaded and uniforms joined appropriately
