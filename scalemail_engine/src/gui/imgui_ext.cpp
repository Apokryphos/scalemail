#include <glm/vec2.hpp>
#include <imgui.h>

namespace ImGui
{
void Value(const char* prefix, const glm::vec2& v) {
    Text("%s: %.2f, %.2f", prefix, v[0], v[1]);
}
}