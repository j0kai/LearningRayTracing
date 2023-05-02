#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
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
			ImGui::Begin("Sphere Color Controller");
			if (ImGui::ColorPicker3("Color Picker", m_SphereColorArr))
			{
				m_Renderer.SetSphereColor(glm::vec3(m_SphereColorArr[0], m_SphereColorArr[1], m_SphereColorArr[2]));
			}
			ImGui::End();

			ImGui::Begin("Light Direction Controller");
			if (ImGui::SliderFloat("X Axis", &m_LightDir.x, -5.0f, 5.0f))
			{
				m_Renderer.SetLightDirection(glm::vec3(m_LightDir.x, m_LightDir.y, m_LightDir.z));
			}
			if (ImGui::SliderFloat("Y Axis", &m_LightDir.y, -5.0f, 5.0f))
			{
				m_Renderer.SetLightDirection(glm::vec3(m_LightDir.x, m_LightDir.y, m_LightDir.z));
			}
			if (ImGui::SliderFloat("Z Axis", &m_LightDir.z, -5.0f, 5.0f))
			{
				m_Renderer.SetLightDirection(glm::vec3(m_LightDir.x, m_LightDir.y, m_LightDir.z));
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
		m_Renderer.Render();

		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	bool m_IsEditable = true;

	glm::vec3 m_LightDir = m_Renderer.GetLightDirection();

	glm::vec3 m_SphereColor = m_Renderer.GetSphereColor();
	float m_SphereColorArr[3] { m_SphereColor.r, m_SphereColor.g, m_SphereColor.b };

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