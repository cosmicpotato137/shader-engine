#include "ComputeObject.h"
#include "Log.h"
#include "imgui.h"
#include "re_core.h"

ComputeObject::ComputeObject(
    const std::string &name, const std::string &shaderPath)
    : name(name) {
  shader = std::make_shared<ComputeShader>(name);
  this->shaderPath =
      strcpy(this->shaderPath, (SHADER_DIR + "/compute/hello.compute").c_str());
  SetShader(shaderPath);
}

void ComputeObject::SetShader(const std::string &shaderPath) {
  std::string oldPath = this->shaderPath;
  shader->Drop();
  shader->Cleanup();
  if (!shader->Init(shaderPath))
    shader->Init(oldPath);
  else {
    this->shaderPath = strcpy(this->shaderPath, shaderPath.c_str());
    Console::Log("Shader compiled sucessfully");
  }
}

void ComputeObject::RecompileShader() {
  std::string shaderPath = shader->GetFilePath();
  if (shader->Init(std::string(shaderPath)))
    Console::Log("Shader compiled sucessfully");
}

void ComputeObject::Step() { step = true; }

void ComputeObject::ToggleStop() { pause = !pause; }

void ComputeObject::Start() { pause = false; }

void ComputeObject::Stop() { pause = true; }

void ComputeObject::Render() {
  if (shader->HasUniform("_reset"))
    shader->SetUniform("_reset", reset);
  if (shader->HasUniform("_pause"))
    shader->SetUniform("_pause", pause && !step);

  // Deploy shader
  shader->Use();

  step = false;
  reset = false;
}

void ComputeObject::ImGuiRender() { ShaderInfoWindow(); }

void ComputeObject::ShaderInfoWindow() {
  ImGui::Begin((name + " Shader Info").c_str());

  // Get shader from file

  ImGui::Text("Get shader from file");
  ImGui::Spacing();

  ImGui::InputText("Input new path", shaderPath, 256);
  if (ImGui::Button("Set shader path", ImVec2(150, 25)))
    SetShader(shaderPath);

  if (ImGui::Button("Recompile shader", ImVec2(150, 25)))
    RecompileShader();

  // Get uniforms with imgui
  ImGui::Spacing();
  ImGui::Text("Uniforms");
  ImGui::Spacing();

  std::map<std::string, ptr<Uniform>> uniforms = shader->GetUniforms();
  for (auto iter = uniforms.begin(); iter != uniforms.end(); ++iter) {
    std::string name = iter->second->GetName();
    uniform_types value = iter->second->GetValue();

    if (iter->second->GetHide() || iter->second->GetType() == Error)
      continue;

    struct Visitor {
      ptr<Uniform> uniform;

      Visitor(ptr<Uniform> uniform) : uniform(uniform) {}

      void operator()(bool &value) {
        ImGui::Checkbox(std::string("##" + uniform->GetName()).c_str(), &value);
      }
      void operator()(GLint &value) {
        int v = value;
        ImGui::DragInt(
            std::string("##" + uniform->GetName()).c_str(),
            &v,
            1.0f,
            -INT_MAX,
            INT_MAX);
        value = v;
      }
      void operator()(GLuint &value) {
        int v = value;
        ImGui::DragInt(
            std::string("##" + uniform->GetName()).c_str(),
            &v,
            1.0f,
            0,
            INT_MAX);
        value = v;
      }
      void operator()(GLfloat &value) {
        ImGui::DragFloat(
            std::string("##" + uniform->GetName()).c_str(),
            &(float)value,
            .1f,
            -FLT_MAX,
            FLT_MAX);
      }
      void operator()(glm::vec2 &value) {
        ImGui::DragFloat2(
            std::string("##" + uniform->GetName()).c_str(),
            glm::value_ptr(value),
            .1f,
            -FLT_MAX,
            FLT_MAX);
      }
      void operator()(glm::vec3 &value) {
        if (uniform->GetType() == Col3)
          ImGui::ColorEdit3(
              std::string("##" + uniform->GetName()).c_str(),
              glm::value_ptr(value));
        else
          ImGui::DragFloat3(
              std::string("##" + uniform->GetName()).c_str(),
              glm::value_ptr(value),
              .1f,
              -FLT_MAX,
              FLT_MAX);
      }
      void operator()(glm::vec4 &value) {
        if (uniform->GetType() == Col4)
          ImGui::ColorEdit4(
              std::string("##" + uniform->GetName()).c_str(),
              glm::value_ptr(value));
        else
          ImGui::DragFloat4(
              std::string("##" + uniform->GetName()).c_str(),
              glm::value_ptr(value),
              .1f,
              -FLT_MAX,
              FLT_MAX);
      }
      void operator()(const glm::mat4 &value) {}
    };

    // Slider column
    ImGui::Columns(2, 0, false);
    ImGui::SetColumnWidth(0, 100.0f);
    std::visit(Visitor(iter->second), value);

    // Text column
    ImGui::NextColumn();
    ImGui::Text(name.c_str());
    ImGui::Columns();

    iter->second->SetValue(value);
  }

  if (shader->HasUniform("_scale"))
    ImGui::Text("Zoom: %f", shader->GetUniforms()["_scale"]->GetValue<float>());
  if (shader->HasUniform("_mouse_position")) {
    glm::vec2 pos =
        shader->GetUniforms()["_mouse_position"]->GetValue<glm::vec2>();
    pos = glm::vec2(roundf(pos.x * 100), roundf(pos.y * 100)) /
          100.0f; /* Result: 37.78 */
    ImGui::Text("MousePosition: (%f, %f)", pos.x, pos.y);
  }

  if (ImGui::Button(pause ? "Play" : "Pause", ImVec2(150, 25)))
    pause = !pause;
  if (ImGui::Button("Step", ImVec2(150, 25)))
    step = true;

  if ((shader->HasUniform("_reset") || shader->HasUniform("_time")) &&
      ImGui::Button("Reset shader", ImVec2(150, 25))) {
    reset = true;
  }

  ImGui::End();
}
