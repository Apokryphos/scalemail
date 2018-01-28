#include "light_system.hpp"

namespace ScaleMail
{
//	============================================================================
glm::vec4 LightSystem::getAmbientColor() const {
    return mAmbientColor;
}

//	============================================================================
void LightSystem::setAmbientColor(const glm::vec4 ambientColor) {
    mAmbientColor = ambientColor;
}
}