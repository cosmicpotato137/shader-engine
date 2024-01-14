#pragma once
#include <fstream>

class Object {
public:
  // Generic update
  void Update(float dt) {}

  // Render images using opengl
  void Render() {}

  // Render data using imgui
  void ImGuiRender() {}

  static void Save(Object &obj, const std::string &filepath);
  static void Load(Object &obj, const std::string &filepath);

protected:
  virtual void Serialize(std::ofstream &ofs) = 0;
  virtual void Deserialize(std::ifstream &ifs) = 0;
};