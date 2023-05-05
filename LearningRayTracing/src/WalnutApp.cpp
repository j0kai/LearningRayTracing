#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Camera.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		:m_Camera(45.0f, 0.1f, 1000.0f) 
	{
		{
			Sphere sphere;
			sphere.Position = { 0.0f, 0.0f, 0.0f };
			sphere.Radius = 0.5f;
			sphere.Albedo = { 1.0f, 1.0f, 1.0f };
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 1.0f, 0.0f, -5.0f };
			sphere.Radius = 1.5f;
			sphere.Albedo = { 0.2f, 0.3f, 0.6f };
			m_Scene.Spheres.push_back(sphere);
		}
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if(m_IsEditable)
		{
			if (ImGui::Button("Disable Editing"))
					m_IsEditable = false;
		}
		else
		{
			if (ImGui::Button("Enable Editing"))
				m_IsEditable = true;
		}

		ImGui::End();

		if (m_IsEditable)
		{
			ImGui::Begin("Sphere Properties");
			
			for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
			{
				ImGui::PushID(i);

				Sphere& sphere = m_Scene.Spheres[i];
				ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.05f);
				ImGui::DragFloat("Radius", &sphere.Radius, 0.05f);
				ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo));

				ImGui::PopID();
			}

			ImGui::End();

			ImGui::Begin("Light Direction Controller");
			if (ImGui::DragFloat3("Light Direction", glm::value_ptr(m_LightDir), 0.1f, -10.0f, 10.0f))
			{
				m_Renderer.SetLightDirection(glm::vec3(m_LightDir.x, m_LightDir.y, m_LightDir.z));
			}
			ImGui::End();

			ImGui::Begin("Generate Sphere");
			if (ImGui::Button("Generate Sphere"))
			{
				Sphere sphere;
				m_Scene.Spheres.push_back(sphere);
			}

			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;


		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1, 0));
		 
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	bool m_IsEditable = true;

	glm::vec3 m_LightDir = m_Renderer.GetLightDirection();

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Learning Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}