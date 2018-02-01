#include "camera.hpp"
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
//  ============================================================================
Camera::Camera(float width, float height, float zoom) {
	this->zoom = zoom;
	this->setSize(width, height);
}

//  ============================================================================
glm::mat4 Camera::getView() const {
	return
		glm::scale(glm::vec3(this->zoom, this->zoom, 1.0f)) *
		glm::translate(glm::vec3(-this->position, 0.0f));
}

//  ============================================================================
void Camera::setSize(float width, float height) {
	mWidth = width;
	mHeight = height;

	mProjection = glm::ortho(0.0f, mWidth, mHeight, 0.0f, 0.0f, 1.0f);
}

//  ============================================================================
glm::vec2 Camera::unproject(const glm::vec2& point) const
{
	//	Flip since OpenGL uses 0,0 for the bottom left corner
	const glm::vec4 viewport(0.0f, 0.0f, mWidth, mHeight);

	glm::vec3 result = glm::unProject(
		glm::vec3(point.x, mHeight - point.y, 0),
		glm::mat4(1.0f),
		mProjection * this->getView(),
		viewport);

	return glm::vec2(result.x, result.y);
}
}