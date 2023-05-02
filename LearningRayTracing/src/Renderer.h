#pragma once

#include "Walnut/Image.h"
#include "glm/glm.hpp"

#include <memory>

class Renderer
{
public:
	Renderer()= default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	const glm::vec3 GetSphereColor() const { return m_SphereColor; }
	void SetSphereColor(const glm::vec3& color) { m_SphereColor = color; }

	const glm::vec3 GetLightDirection() const { return m_LightDirection; }
	void SetLightDirection(const glm::vec3& lightDir) { m_LightDirection = lightDir; }

private:
	glm::vec4 PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	glm::vec3 m_SphereColor{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_LightDirection{ -1.0f, -1.0f, -1.0f };
};