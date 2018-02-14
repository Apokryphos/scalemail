#include "camera.hpp"
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
//  ============================================================================
Camera::Camera(float width, float height, float zoom)
	: mZoom(zoom), mBounds(0, 0, 2048, 2048) {
	this->setSize(width, height);
}

//  ============================================================================
Rectangle Camera::getBounds() const {
	return mBounds;
}

//  ============================================================================
glm::vec2 Camera::getPosition() const {
	return mPosition;
}

//  ============================================================================
glm::mat4 Camera::getProjection() const {
	return mProjection;
}

//  ============================================================================
glm::vec2 Camera::getSize() const {
	return glm::vec2(mWidth, mHeight);
}

//  ============================================================================
glm::mat4 Camera::getView() const {
	glm::vec2 position =
		mPosition -
		glm::vec2(mWidth / 2, mHeight / 2) *
		(1 / mZoom);

	return
		glm::scale(glm::vec3(mZoom, mZoom, 1.0f)) *
		glm::translate(glm::vec3(-position, 0.0f));
}

//  ============================================================================
float Camera::getZoom() const {
	return mZoom;
}

//  ============================================================================
void Camera::setBounds(const Rectangle bounds) {
	mBounds = bounds;
	this->setPosition(mPosition);
}

//  ============================================================================
void Camera::setPosition(glm::vec2 position) {
	float width = mWidth / mZoom;
	float height = mHeight / mZoom;

	const float halfWidth = width / 2;
	const float halfHeight = height / 2;

	if (mBounds.width <= width) {
		//	Bounds are smaller than camera so use center of bounds
		//	as camera center.
		position.x = mBounds.getCenterX();
	} else {
		float cameraLeft = position.x - halfWidth;
		float cameraRight = position.x + halfWidth;

		if (cameraLeft < mBounds.x) {
			position.x = mBounds.x + halfWidth;
		} else if (cameraRight > mBounds.getRight()) {
			position.x = mBounds.getRight() - halfWidth;
		}
	}

	if (mBounds.height <= height) {
		//	Bounds are smaller than camera so use center of bounds
		//	as camera center.
		position.y = mBounds.getCenterY();
	} else {
		float cameraTop = position.y - halfHeight;
		float cameraBottom = position.y + halfHeight;

		if (cameraTop < mBounds.y) {
			position.y = mBounds.y + halfHeight;
		} else if (cameraBottom > mBounds.getBottom()) {
			position.y = mBounds.getBottom() - halfHeight;
		}
	}

	mPosition = position;
}

//  ============================================================================
void Camera::setSize(float width, float height) {
	mWidth = width;
	mHeight = height;

	mProjection = glm::ortho(0.0f, mWidth, mHeight, 0.0f, -1.0f, 1.0f);

	this->setPosition(mPosition);
}

//  ============================================================================
void Camera::setZoom(float zoom) {
	mZoom = zoom;
	this->setPosition(mPosition);
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