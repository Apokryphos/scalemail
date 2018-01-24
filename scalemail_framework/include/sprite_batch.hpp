#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace ScaleMail
{
class SpriteBatch
{
public:
    void buildVertexData(
        std::vector<glm::vec3> position
    );
};
}