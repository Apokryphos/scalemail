#include "camera.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include <glm/glm.hpp>

namespace ScaleMail
{
//	============================================================================
void render(GLFWwindow* window, Camera& camera, glm::vec4 ambientColor) {
    GameWindow gameWindow = {};
    gameWindow.window = window;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwGetFramebufferSize(window, &gameWindow.width, &gameWindow.height);

    renderLight(gameWindow, camera, ambientColor);
}
}