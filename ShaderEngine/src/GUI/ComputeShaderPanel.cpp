#include "ComputeShaderPanel.h"
#include "Core/Application.h"
#include "GUI/ScenePanel.h"

ComputeShaderPanel::ComputeShaderPanel(std::shared_ptr<ComputeShader> shader)
    : Panel("Compute Shader"), m_Shader(shader) {
  if (m_Shader != nullptr) {
    m_Name = m_Shader->GetName();
  }
}

ComputeShaderPanel::ComputeShaderPanel(const ComputeShaderPanel &other)
    : Panel(other.GetName()), m_Shader(other.m_Shader) {}

void ComputeShaderPanel::SetRenderTargets(const Renderer &renderer) {
  if (m_Shader == nullptr)
    return;

  // Required image out
  if (m_Shader->HasUniform("imageOut")) {
    renderer.GetRenderTarget()->GetTexture()->BindCompute(0);
    (*m_Shader)["imageOut"]->SetValue(0);
  } else {
    Console::Error(
        "Shader %s does not have imageOut uniform",
        m_Shader->GetName().c_str());
  }

  // Optional image in
  if (m_Shader->HasUniform("imageIn")) {
    renderer.GetSwapTarget()->GetTexture()->BindCompute(1);
    (*m_Shader)["imageIn"]->SetValue(1);
  }
}

void ComputeShaderPanel::SetWorkGroups(const Renderer &renderer) {
  if (m_Shader == nullptr)
    return;

  glm::vec2 size = renderer.GetViewportSize();
  m_Shader->SetWorkGroups(size.x / 8, size.y / 8, 1);
}

void ComputeShaderPanel::Render(const Renderer &ren) {
  if (m_Shader == nullptr)
    return;

  SetWorkGroups(ren);
  SetRenderTargets(ren);
  m_Shader->Use();

  m_Step = false;
  m_Reset = false;
}

void ComputeShaderPanel::Update(double dt) {
  if (m_Shader == nullptr)
    return;

  if (!m_Pause && !m_Step) {
    m_Time += dt;
    if (m_Shader->HasUniform("_time"))
      (*m_Shader)["_time"]->SetValue(m_Time);
  } else if (m_Pause && m_Step) {
    m_Time += .01f;
    if (m_Shader->HasUniform("_time"))
      (*m_Shader)["_time"]->SetValue(m_Time);
  }

  // Set user input uniforms
  Application *app = Application::GetInstance();
  if (m_Shader->HasUniform("_mouse_position")) {
    ScenePanel *scenePanel = ScenePanel::GetScenePanel(0);
    ImVec2 pos = {};
    if (scenePanel != nullptr) {
      pos = scenePanel->GetPanelCursorPos();
      pos.y = scenePanel->GetPanelContentSize().y - pos.y;
    }

    (*m_Shader)["_mouse_position"]->SetValue(glm::vec2{pos.x, pos.y});
  }

  if (m_Shader->HasUniform("_lmb_down"))
    (*m_Shader)["_lmb_down"]->SetValue(
        app->GetMouseButton(GLFW_MOUSE_BUTTON_1));

  if (m_Shader->HasUniform("_rmb_down"))
    (*m_Shader)["_rmb_down"]->SetValue(
        app->GetMouseButton(GLFW_MOUSE_BUTTON_2));
}

void ComputeShaderPanel::ImGuiRender() {
  if (m_Shader == nullptr)
    return;

  ImGui::Begin("Shader Info");

  m_Size = ImGui::GetWindowSize();
  m_Position = ImGui::GetWindowPos();

  // Shader name and update
  HeaderOperations();

  // Get uniforms with imgui
  ImGui::Text("Uniforms");
  ImGui::Spacing();

  // Iterate over all non-hidden uniforms
  UniformVisitor();

  // Custom uniforms for user input and pause/play/step
  CustomUniforms();

  ImGui::End();
}

void ComputeShaderPanel::SetShader(std::shared_ptr<ComputeShader> shader) {
  m_Shader = shader;
  m_Name = m_Shader->GetName();
}

ComputeShaderPanel &
ComputeShaderPanel::operator=(const ComputeShaderPanel &other) {
  if (this != &other) {       // protect against invalid self-assignment
    Panel::operator=(other);  // call base class assignment operator
    m_Shader = other.m_Shader;
  }
  return *this;
}

void ComputeShaderPanel::UniformVisitor() {
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

    iter->second->SetValue(value);
  }
}

void ComputeShaderPanel::HeaderOperations() {
  char buffer[256];
  std::strncpy(buffer, m_Shader->GetName().c_str(), sizeof(buffer));
  buffer[sizeof(buffer) - 1] = 0;  // Ensure null-termination

  if (ImGui::InputText("Shader Name", buffer, sizeof(buffer))) {
    m_Shader->SetName(std::string(buffer));
  }

  ImGui::Spacing();

  if (ImGui::Button("Load Compute Shader")) {
    std::string path = m_FileDialog.Show();
    if (path != "") {
      if (m_Shader->Init(path)) {
        Console::Log("Shader %s loaded", m_Shader->GetName().c_str());
      }
    }
  }

  ImGui::Spacing();
  if (ImGui::Button("Reload Compute Shader")) {
    if (m_Shader->GetFilePath() != "") {
      if (m_Shader->ReInit()) {
        Console::Log("Shader %s reloaded", m_Shader->GetName().c_str());
      }
    }
  }
}

void ComputeShaderPanel::CustomUniforms() {
  // User input uniforms
  Application *app = Application::GetInstance();
  if (m_Shader->HasUniform("_mouse_position")) {
    glm::vec2 pos =
        m_Shader->GetUniform("_mouse_position")->GetValue<glm::vec2>();
    // glm::vec2 pos = app->GetCursorPosition();
    ImGui::Text("MousePosition: (%f, %f)", pos.x, pos.y);
  }

  // Check for unique uniforms among the hidden ones
  if (m_Shader->HasUniform("_scale")) {
    // Slider from 0 to 10 in increments of 0.1
    float scl = (*m_Shader)["_scale"]->GetValue<float>();
    ImGui::SliderFloat("Zoom", &scl, 0.0f, 10.0f, "%.1f");
    (*m_Shader)["_scale"]->SetValue(scl);
  }

  if (m_Shader->HasUniform("_pause")) {
    OnPause();
    OnStep();
    (*m_Shader)["_pause"]->SetValue(m_Pause && !m_Step);
  }

  // TODO implement _time
  OnReset();
  if (m_Shader->HasUniform("_reset")) {
    (*m_Shader)["_reset"]->SetValue(m_Reset);
  }
}

void ComputeShaderPanel::OnReset() {
  if (ImGui::Button("Reset shader", ImVec2(150, 25))) {
    m_Reset = true;
    m_Time = 0.0f;
  }
}

void ComputeShaderPanel::OnPause() {
  if (ImGui::Button(m_Pause ? "Play" : "Pause", ImVec2(150, 25)))
    m_Pause = !m_Pause;
}

void ComputeShaderPanel::OnStep() {
  if (ImGui::Button("Step", ImVec2(150, 25)))
    m_Step = true;
}