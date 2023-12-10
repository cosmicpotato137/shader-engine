#include "Simulation.h"
#include <iostream>

Simulation::Simulation() {}

void Simulation::Init(ptr<RenderTexture> canvas)
{
    this->canvas = canvas;
}

void Simulation::AddAgent(const glm::vec2& initialPosition, const glm::vec2& initialVelocity) {
    agents.emplace_back(initialPosition, initialVelocity);
}

void Simulation::Update(float deltaTime) {
    for (Agent& agent : agents) {
        glm::vec2 pos = agent.GetPosition();
        glm::vec2 vel = agent.GetVelocity();

        // flip velocity if agent is at the edge of the texture
        if (pos.x <= 0 || pos.x >= canvas->GetWidth())
        {
            pos.x = glm::clamp(pos.x, 0.0f, (float)canvas->GetWidth());
            vel = { -vel.x, vel.y };
        }
        if (pos.y <= 0 || pos.y >= canvas->GetHeight())
        {
            pos.x = glm::clamp(pos.y, 0.0f, (float)canvas->GetHeight());
            vel = { vel.x, -vel.y };
        }
        agent.SetVelocity(vel);

        agent.Update(deltaTime);
    }
}

void Simulation::Render()
{
    //canvas->BeginRender();
    //canvas->Clear();

    for (Agent& agent : agents) {
        glm::vec2 pos = agent.GetPosition();

        if (pos.x >= 0 && pos.x < canvas->GetWidth() &&
            pos.y >= 0 && pos.y < canvas->GetHeight())
        {
            canvas->GetTexture()->WritePixel(pos.x, pos.y, 255, 255, 255);
            //Renderer::DrawLine(agent.GetLastPosition() / canvas->GetSize(), pos / canvas->GetSize());
        }
    }
    //canvas->EndRender();
}
