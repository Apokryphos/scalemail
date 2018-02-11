#pragma once

#include "rectangle.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace ScaleMail
{
class Camera
{
	float mWidth;
	float mHeight;
	float mZoom;
	glm::vec2 mPosition;
	Rectangle mBounds;
	glm::mat4 mProjection;

public:
	Camera(float width, float height, float zoom = 2.0f);
	glm::vec2 getPosition() const;
	glm::mat4 getProjection() const;
	glm::vec2 getSize() const;
	glm::mat4 getView() const;
	float getZoom() const;
	void setBounds(const Rectangle bounds);
	void setPosition(const glm::vec2 position);
	void setSize(float width, float height);
	void setZoom(const float zoom);
	glm::vec2 unproject(const glm::vec2& point) const;
};
}