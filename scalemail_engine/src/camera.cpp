#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ScaleMail
{
//  ============================================================================
Camera::Camera(float zoom) {
    this->zoom = zoom;
}

//  ============================================================================
glm::mat4 Camera::getView() {
    return
        glm::scale(glm::mat4(1.0f), glm::vec3(this->zoom, this->zoom, 0.0f)) *
        glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f));
}
}