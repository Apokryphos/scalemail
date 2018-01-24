#include "camera.hpp"
#include "font.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace ScaleMail
{
//	============================================================================
void render(GLFWwindow* window, Camera& camera, glm::vec4 ambientColor) {
    GameWindow gameWindow = {};
    gameWindow.window = window;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwGetFramebufferSize(window, &gameWindow.width, &gameWindow.height);

    renderLight(gameWindow, camera, ambientColor);
    renderSprites(gameWindow, camera);
    renderTransition();
    renderText(gameWindow);
}
}