#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace ScaleMail
{
class Camera
{
public:
    Camera(float zoom = 2.0f);
    glm::vec2 position;
    glm::mat4 getView() const;
    float zoom;
};
}