#include "gui/gui.hpp"
#include "ai_system.hpp"
#include "ambient_light.hpp"
#include "asset_manager.hpp"
#include "blend.hpp"
#include "camera.hpp"
#include "camera_system.hpp"
#include "font.hpp"
#include "game.hpp"
#include "game_state.hpp"
#include "game_window.hpp"
#include "gl_headers.hpp"
#include "map.hpp"
#include "map_render.hpp"
#include "mesh.hpp"
#include "light.hpp"
#include "quad_shader.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "sprite.hpp"
#include "transition.hpp"
#include "trigger_system.hpp"
#include "vertex_data.hpp"
#include "world.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>

namespace ScaleMail
{
static Mesh debugLineMesh;
static std::vector<float> debugLineVertexData;
static LineShader debugLineShader;

static Mesh quadMesh;
static QuadShader colorQuadShader;
static QuadShader particleShader;

static Mesh visQuadMesh;
static std::vector<float> visQuadVertexData;

//	============================================================================
void initializeRender(AssetManager& assetManager) {
	quadMesh = assetManager.getQuadMesh();
	colorQuadShader = assetManager.getColorQuadShader();
	particleShader = assetManager.getParticleShader();

	assetManager.initializeMesh(visQuadMesh,
								VertexDefinition::POSITION2_COLOR4);

	debugLineShader = assetManager.getLineShader();
	assetManager.initializeMesh(
		debugLineMesh,
		VertexDefinition::POSITION2_COLOR4,
		5000,
		GL_LINES);
	debugLineVertexData.reserve(5000);

	initializeFont(assetManager);
	initializeTransition(assetManager);
	initializeSprites(assetManager);
	initializeMapMesh(assetManager);
	initializeAmbientLights(assetManager);

	if (assetManager.getRenderCaps().fboSupported) {
		initializeLight(assetManager);
	}
}

//  ============================================================================
static void buildVisQuads(const std::vector<CameraVisibility>& visibility) {
	visQuadVertexData.resize(0);

	for (const auto& v : visibility) {
		glm::vec2 position = glm::vec2(v.bounds.x, v.bounds.y);
		glm::vec2 size = glm::vec2(v.bounds.width, v.bounds.height);

		glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f - v.alpha);

		addQuadVertexData(visQuadVertexData, position, size, color);
	}

	setMeshVertexData(visQuadMesh, visQuadVertexData);
}

//	============================================================================
void updateStencilBuffer(CameraSystem& cameraSystem, Camera& camera) {
	const auto& vis = cameraSystem.getVisibility();

	buildVisQuads(vis);

	//	Prepare stencil buffer writes
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	//	Draw camera bounds quad
	glUseProgram(colorQuadShader.id);

	for (const auto& v : vis) {
		if (v.alpha <= 0.0f) {
			continue;
		}

		Rectangle cameraBounds = v.bounds;

		glm::mat4 quadWorld =
			glm::translate(glm::vec3(
				cameraBounds.x + cameraBounds.width * 0.5f,
				cameraBounds.y + cameraBounds.height * 0.5f, 0.0f)) *
			glm::scale(glm::vec3(
				cameraBounds.width * 0.5f,
				cameraBounds.height * 0.5f, 1.0f));

		const glm::mat4 mvp =
			camera.getProjection() * camera.getView() * quadWorld;

		glUniformMatrix4fv(colorQuadShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);

		drawMesh(quadMesh);
	}

	//	Disable writes stencil buffer
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
}

//	============================================================================
void renderVisibility(CameraSystem& cameraSystem, Camera& camera) {
	const auto& vis = cameraSystem.getVisibility();

	blendAlpha();

	const glm::mat4 mvp =
		camera.getProjection() * camera.getView();

	//	Draw camera bounds quad
	glUseProgram(colorQuadShader.id);
	glUniformMatrix4fv(colorQuadShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);

	for (const auto& v : vis) {
		if (v.alpha <= 0.0f) {
			continue;
		}

		drawMesh(visQuadMesh);
	}
}

//	============================================================================
void renderDebug(Game& game, Camera& camera) {
	//	Clear vertex data
	debugLineVertexData.resize(0);

	if (game.devOptions.enabled) {
		World& world = *game.world;

		if (game.devOptions.drawCollision) {
			world.getPhysicsSystem().drawDebug(debugLineVertexData);
		}

		if (game.devOptions.drawAi) {
			world.getAiSystem().drawDebug(debugLineVertexData);
		}

		if (game.devOptions.drawTriggers) {
			world.getTriggerSystem().drawDebug(debugLineVertexData);
		}

		if (debugLineVertexData.size() > 0) {
			const glm::mat4 mvp = camera.getProjection() * camera.getView();

			setMeshVertexData(debugLineMesh, debugLineVertexData);

			glUseProgram(debugLineShader.id);
			glUniformMatrix4fv(debugLineShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);

			drawMesh(debugLineMesh);
		}
	}
}

//	============================================================================
void renderParticles(World& world, Camera& camera) {
	ParticleSystem& particleSystem = world.getParticleSystem();

	particleSystem.buildVertexData();

	const Mesh& mesh = particleSystem.getMesh();

	const glm::mat4 mvp = camera.getProjection() * camera.getView();

	glUseProgram(particleShader.id);
	glUniformMatrix4fv(particleShader.mvpLocation, 1, GL_FALSE, &mvp[0][0]);

	drawMesh(mesh);
}

//	============================================================================
void render(Game& game, World& world, GameState& gameState,
			float totalElapsedSeconds) {
	if (game.camera == nullptr) {
		return;
	}

	Camera& camera = *game.camera;

	GameWindow& gameWindow = game.gameWindow;

	if (game.renderCaps.fboSupported) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(0, 0, gameWindow.getWidth(), gameWindow.getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	Don't use stencil buffer if the camera is in 3D
	if (!game.devOptions.camera3d) {
		updateStencilBuffer(world.getCameraSystem(), camera);
	}

	renderMap(*world.getMap(), camera, totalElapsedSeconds);
	renderSprites(world.getSpriteSystem(), world.getSpriteEffectSystem(), camera);

	glEnable(GL_DEPTH_TEST);
	blendAlphaAdditive();
	renderParticles(world, camera);
	blendNone();
	glDisable(GL_DEPTH_TEST);

	if (game.renderOptions.lightsEnabled) {
		renderLight(gameWindow, camera, world.getLightSystem());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	renderVisibility(world.getCameraSystem(), camera);

	renderTransition();

	//	Debug drawing
	renderDebug(game, camera);

	glDisable(GL_STENCIL_TEST);

	gameState.draw(game, camera);

	//	Draw GUI
	if (!game.devOptions.enabled || !game.devOptions.hideGui) {
		if (game.gui != nullptr) {
			game.gui->draw(game);
		}

		renderText(gameWindow);
	}

	glfwSwapBuffers(gameWindow.getGlfwWindow());
}
}