#pragma once

#include "entity_system.hpp"
#include <glm/vec4.hpp>
#include <vector>

namespace ScaleMail
{
struct Entity;
class EntityManager;
class SpriteBatch;
class SpriteSystem;

struct SpriteEffectComponent {
	SpriteEffectComponent(const int index) { this->index = index; }
	int index;
};

struct SpriteEffectComponentData {
	float blinkCount;
	float blinkTicks;
	float blinkDuration;
	glm::vec4 blinkColor;
};

class SpriteEffectSystem : public EntitySystem
{
private:
	std::vector<SpriteEffectComponentData> mData;

protected:
	virtual void createComponent() override;
	virtual void destroyComponent(int index) override;

public:
	SpriteEffectSystem(EntityManager& entityManager, int maxComponents = 10000);
	SpriteEffectComponent getComponent(const Entity& entity) const;
	void blink(const SpriteEffectComponent& cmpnt);
	void buildVertexData(SpriteBatch& spriteBatch, SpriteSystem& spriteSystem);
	void setBlinkDuration(const SpriteEffectComponent& cmpnt, const float duration);
	void setBlinkColor(const SpriteEffectComponent& cmpnt, const glm::vec4 color);
	void update(float elapsedSeconds, SpriteSystem& spriteSystem);
};
}