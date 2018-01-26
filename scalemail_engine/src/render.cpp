#include "camera.hpp"
#include "font.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "light.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <iostream>

namespace ScaleMail
{
//	============================================================================
void render(GLFWwindow* window, World& world, Camera& camera,
            glm::vec4 ambientColor) {
    GameWindow gameWindow = {};
    gameWindow.window = window;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwGetFramebufferSize(window, &gameWindow.width, &gameWindow.height);

    renderSprites(gameWindow, world.getSpriteSystem(), camera);
    renderLight(gameWindow, camera, ambientColor);
    renderTransition();
    renderText(gameWindow);
}
}