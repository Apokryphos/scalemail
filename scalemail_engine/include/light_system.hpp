#pragma once

#include <glm/vec4.hpp>

namespace ScaleMail
{
class LightSystem
{
    glm::vec4 mAmbientColor;

public:
    glm::vec4 getAmbientColor() const;
    void setAmbientColor(const glm::vec4 ambientColor);
};
}