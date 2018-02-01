#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace ScaleMail
{
class Camera
{
	float mWidth;
	float mHeight;
	glm::mat4 mProjection;

public:
	Camera(float width, float height, float zoom = 2.0f);
	glm::vec2 position;
	glm::mat4 getView() const;
	void setSize(float width, float height);
	glm::vec2 unproject(const glm::vec2& point) const;
	float zoom;
};
}