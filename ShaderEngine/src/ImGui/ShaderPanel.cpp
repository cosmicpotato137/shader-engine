#include "ShaderPanel.h"

ShaderPanel::ShaderPanel(std::shared_ptr<Shader> shader) : m_Shader(shader) {
  if (shader == nullptr)
    return;
  m_Name = shader->GetName();
  m_ShaderPath = shader->GetFilePath();
}

void ShaderPanel::ImGuiRender() {
  if (m_Shader == nullptr)
    return;

  ImGui::Begin((m_Name + " Shader Info").c_str());

  float totalWidth = ImGui::GetContentRegionAvail().x;

  // Get shader from file
  // ImGui::Text("Get shader from file");
  // ImGui::Spacing();

  // open Dialog Simple
  // if (ImGui::Button("Load Shader")) {
  //   config.path = ".";
  //   fileDialog.OpenDialog(
  //       "ChooseFileDlgKey", "Choose File", ".shader,.compute", config);
  // }

  // display
  // if (fileDialog.Display("ChooseFileDlgKey")) {
  //   if (fileDialog.IsOk()) {  // action if OK
  //     std::string filePathName = fileDialog.GetFilePathName();
  //     std::string filePath = fileDialog.GetCurrentPath();

  //     SetShader(filePathName);
  //   }

  // close
  // fileDialog.Close();
  // }

  if (ImGui::Button("Recompile shader", ImVec2(150, 25)))
    m_Shader->Init(m_Shader->GetFilePath());

  // Get uniforms with imgui
  ImGui::Spacing();
  ImGui::Text("Uniforms");
  ImGui::Spacing();

  for (auto iter = m_Shader->begin(); iter != m_Shader->end(); ++iter) {
    std::string name = iter->second->GetName();
    uniform_types value = iter->second->GetValue();

    if (iter->second->GetHide() || iter->second->GetType() == Error)
      continue;

    struct Visitor {
      ptr<Uniform> uniform;

      Visitor(ptr<Uniform> uniform) : uniform(uniform) {}

      void operator()(bool &value) {
        ImGui::Checkbox(std::string(uniform->GetName()).c_str(), &value);
      }
      void operator()(GLint &value) {
        int v = value;
        ImGui::DragInt(
            std::string(uniform->GetName()).c_str(),
            &v,
            1.0f,
            -INT_MAX,
            INT_MAX);
        value = v;
      }
      void operator()(GLuint &value) {
        int v = value;
        ImGui::DragInt(
            std::string(uniform->GetName()).c_str(), &v, 1.0f, 0, INT_MAX);
        value = v;
      }
      void operator()(GLfloat &value) {
        ImGui::DragFloat(
            std::string(uniform->GetName()).c_str(),
            (float *)&value,
            .1f,
            -FLT_MAX,
            FLT_MAX);
      }
      void operator()(glm::vec2 &value) {
        ImGui::DragFloat2(
            std::string(uniform->GetName()).c_str(),
            glm::value_ptr(value),
            .1f,
            -FLT_MAX,
            FLT_MAX);
      }
      void operator()(glm::vec3 &value) {
        if (uniform->GetType() == Col3)
          ImGui::ColorEdit3(
              std::string(uniform->GetName()).c_str(), glm::value_ptr(value));
        else
          ImGui::DragFloat3(
              std::string(uniform->GetName()).c_str(),
              glm::value_ptr(value),
              .1f,
              -FLT_MAX,
              FLT_MAX);
      }
      void operator()(glm::vec4 &value) {
        if (uniform->GetType() == Col4)
          ImGui::ColorEdit4(
              std::string(uniform->GetName()).c_str(), glm::value_ptr(value));
        else
          ImGui::DragFloat4(
              std::string(uniform->GetName()).c_str(),
              glm::value_ptr(value),
              .1f,
              -FLT_MAX,
              FLT_MAX);
      }
      void operator()(const glm::mat4 &value) {}
    };

    std::visit(Visitor(iter->second), value);

    // // Slider column
    // ImGui::Columns(2, 0, false);
    // ImGui::SetColumnWidth(0, totalWidth);

    // // Text column
    // ImGui::NextColumn();
    // ImGui::Text(name.c_str());
    // ImGui::Columns();

    iter->second->SetValue(value);
  }

  if (m_Shader->HasUniform("_scale"))
    ImGui::Text("Zoom: %f", (*m_Shader)["_scale"]->GetValue<float>());
  if (m_Shader->HasUniform("_mouse_position")) {
    glm::vec2 pos = (*m_Shader)["_mouse_position"]->GetValue<glm::vec2>();
    pos = glm::vec2(roundf(pos.x * 100), roundf(pos.y * 100)) / 100.0f;
    ImGui::Text("MousePosition: (%f, %f)", pos.x, pos.y);
  }

  if (ImGui::Button(m_Pause ? "Play" : "Pause", ImVec2(150, 25)))
    m_Pause = !m_Pause;
  if (ImGui::Button("Step", ImVec2(150, 25)))
    m_Step = true;

  if ((m_Shader->HasUniform("_reset") || m_Shader->HasUniform("_time")) &&
      ImGui::Button("Reset shader", ImVec2(150, 25))) {
    m_Reset = true;
  }

  ImGui::End();
}

void ShaderPanel::SetShader(std::shared_ptr<Shader> shader) {
  m_Shader = shader;
  m_Name = shader->GetName();
  m_ShaderPath = shader->GetFilePath();
}