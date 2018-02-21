#include "color_util.hpp"
#include "math_util.hpp"
#include "particle_system.hpp"
#include "physics_system.hpp"
#include "random.hpp"
#include "vector_util.hpp"

namespace ScaleMail
{
static const size_t PARTICLE_RESERVE = 10000;

//	============================================================================
static inline glm::vec3 rotate(const glm::vec3& vector, float radians)
{
	return glm::vec3(
		vector.x * std::cos(radians) - vector.y * std::sin(radians),
		vector.x * std::sin(radians) + vector.y * std::cos(radians),
		vector.z);
}

//	============================================================================
static inline bool sortRemoveIndex(const size_t& index1, const size_t& index2) {
	return index2 < index1;
}

//	============================================================================
static ParticleComponent makeComponent(const int index) {
	return ParticleComponent(index);
}

//	============================================================================
ParticleSystem::ParticleSystem(EntityManager& entityManager, int maxComponents)
	: EntitySystem(entityManager, maxComponents) {
	mData.reserve(maxComponents);

	mRemoveParticles.reserve(maxComponents);

	mLife.reserve(PARTICLE_RESERVE);
	mLifetime.reserve(PARTICLE_RESERVE);
	mDecay.reserve(PARTICLE_RESERVE);
	mSize.reserve(PARTICLE_RESERVE);
	mSpeed.reserve(PARTICLE_RESERVE);
	mPositionX.reserve(PARTICLE_RESERVE);
	mPositionY.reserve(PARTICLE_RESERVE);
	mPositionZ.reserve(PARTICLE_RESERVE);
	mDirectionX.reserve(PARTICLE_RESERVE);
	mDirectionY.reserve(PARTICLE_RESERVE);
	mColorR.reserve(PARTICLE_RESERVE);
	mColorG.reserve(PARTICLE_RESERVE);
	mColorB.reserve(PARTICLE_RESERVE);
	mColorA.reserve(PARTICLE_RESERVE);
}

//	============================================================================
void ParticleSystem::buildVertexData() {
	mVertexData.resize(0);

	const size_t particleCount = mPositionX.size();
	for (size_t index = 0; index < particleCount; ++index) {
		const float progress = mLife[index] / mLifetime[index];

		const float halfSize = (mSize[index] * progress) * 0.5f;

		mVertexData.emplace_back(mPositionX[index] - halfSize);
		mVertexData.emplace_back(mPositionY[index] - halfSize);
		mVertexData.emplace_back(mPositionZ[index] + halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);

		mVertexData.emplace_back(mPositionX[index] - halfSize);
		mVertexData.emplace_back(mPositionY[index] + halfSize);
		mVertexData.emplace_back(mPositionZ[index] - halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);

		mVertexData.emplace_back(mPositionX[index] + halfSize);
		mVertexData.emplace_back(mPositionY[index] + halfSize);
		mVertexData.emplace_back(mPositionZ[index] - halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);

		mVertexData.emplace_back(mPositionX[index] + halfSize);
		mVertexData.emplace_back(mPositionY[index] + halfSize);
		mVertexData.emplace_back(mPositionZ[index] - halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);

		mVertexData.emplace_back(mPositionX[index] + halfSize);
		mVertexData.emplace_back(mPositionY[index] - halfSize);
		mVertexData.emplace_back(mPositionZ[index] + halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);

		mVertexData.emplace_back(mPositionX[index] - halfSize);
		mVertexData.emplace_back(mPositionY[index] - halfSize);
		mVertexData.emplace_back(mPositionZ[index] + halfSize);
		mVertexData.emplace_back(mColorR[index]);
		mVertexData.emplace_back(mColorG[index]);
		mVertexData.emplace_back(mColorB[index]);
		mVertexData.emplace_back(mColorA[index]);
	}

	updateMesh(mMesh, mVertexData);
}

//	============================================================================
void ParticleSystem::createComponent() {
	mData.push_back({});
}

//	============================================================================
void ParticleSystem::destroyComponent(int index) {
	swapWithLastElementAndRemove(mData, index);
}

//	============================================================================
ParticleComponent ParticleSystem::getComponent(const Entity& entity) const {
	return makeComponent(this->getComponentIndexByEntity(entity));
}

//	============================================================================
Mesh& ParticleSystem::getMesh() {
	return mMesh;
}

//	============================================================================
void ParticleSystem::initialize(Random& random) {
	mRandom = &random;

	//	Six vertices for every particle quad
	initPositionColorMesh(mMesh, 6 * PARTICLE_RESERVE);
}

//	============================================================================
void ParticleSystem::setData(const ParticleComponent& cmpnt,
							 const ParticleComponentData& data) {
	mData[cmpnt.index] = data;
}

//	============================================================================
void ParticleSystem::update(PhysicsSystem& physicsSystem, float elapsedSeconds) {
	mRemoveParticles.resize(0);

	//	Update particles
	size_t particleCount = mLife.size();
	for (size_t index = 0; index < particleCount; ++index) {
		mLife[index] -= mDecay[index] * elapsedSeconds;

		const float progress = mLife[index] / mLifetime[index];
		mColorA[index] = progress;

		if (mLife[index] <= 0.0f) {
			mRemoveParticles.emplace_back(index);
		}
	}

	//	Sort indices in descending order
	std::sort(mRemoveParticles.begin(), mRemoveParticles.end(), sortRemoveIndex);

	//	Remove dead particles
	for (const auto index : mRemoveParticles) {
		swapWithLastElementAndRemove(mLife, index);
		swapWithLastElementAndRemove(mLifetime, index);
		swapWithLastElementAndRemove(mDecay, index);
		swapWithLastElementAndRemove(mSize, index);
		swapWithLastElementAndRemove(mSpeed, index);
		swapWithLastElementAndRemove(mPositionX, index);
		swapWithLastElementAndRemove(mPositionY, index);
		swapWithLastElementAndRemove(mPositionZ, index);
		swapWithLastElementAndRemove(mDirectionX, index);
		swapWithLastElementAndRemove(mDirectionY, index);
		swapWithLastElementAndRemove(mColorR, index);
		swapWithLastElementAndRemove(mColorG, index);
		swapWithLastElementAndRemove(mColorB, index);
		swapWithLastElementAndRemove(mColorA, index);
	}

	//	Update particle positions
	particleCount = mLife.size();
	for (size_t index = 0; index < particleCount; ++index) {
		mPositionX[index] += mDirectionX[index] * mSpeed[index] * elapsedSeconds;
		mPositionY[index] += mDirectionY[index] * mSpeed[index] * elapsedSeconds;
		mPositionZ[index] += mDirectionZ[index] * mSpeed[index] * elapsedSeconds;
	}

	//	Update emitters
	const size_t emitterCount = mData.size();
	for (size_t index = 0; index < emitterCount; ++index) {
		ParticleComponentData& data = mData[index];

		if (data.delay > 0.0f) {
			data.delay -= elapsedSeconds;
			continue;
		}

		data.ticks += elapsedSeconds;

		if (data.ticks >= data.interval) {
			data.ticks = 0.0f;

			//	Use physics system position
			const Entity& entity = this->getEntityByComponentIndex(index);
			PhysicsComponent physicsCmpnt = physicsSystem.getComponent(entity);
			glm::vec2 position = physicsSystem.getPosition(physicsCmpnt);

			position.y += 0.001f;

				//	Create new particles
			for (int p = 0; p < data.emitCount; ++p) {
				glm::vec3 direction = rotate(
					data.direction,
					mRandom->nextFloat(
						-data.spread * 0.5f, data.spread * 0.5f));

				mDirectionX.emplace_back(direction.x);
				mDirectionY.emplace_back(direction.y);
				mDirectionZ.emplace_back(direction.z);

				float radialOffset = mRandom->nextFloat(0.0f, data.radius);

				float offsetX = mRandom->nextFloat(
					-data.width * 0.5f, data.width * 0.5f);

				float offsetY = mRandom->nextFloat(
					-data.height * 0.5f, data.height * 0.5f);

				mPositionX.emplace_back(position.x + offsetX + direction.x * radialOffset);

				mPositionY.emplace_back(position.y + offsetY + direction.y * radialOffset);

				mPositionZ.emplace_back(10.0f);

				float speed = mRandom->nextFloat(data.minSpeed, data.maxSpeed);
				mSpeed.emplace_back(speed);

				float size = mRandom->nextFloat(data.minSize, data.maxSize);
				mSize.emplace_back(size);

				mLife.emplace_back(data.life);
				mLifetime.emplace_back(data.life);
				mDecay.emplace_back(data.decay);

				//	Convert color to HSL
				glm::vec4 hsl = rgbToHsl(data.color);

				//	Vary hue
				hsl[0] = wrap(hsl[0] + mRandom->nextFloat(-0.025f, 0.025f));

				//	Vary lightness
				hsl[2] = wrap(hsl[2] + mRandom->nextFloat(-0.3f, 0.1f));

				//	Convert color back to RGB
				glm::vec4 rgb = hslToRgb(hsl);

				mColorR.emplace_back(rgb.r);
				mColorG.emplace_back(rgb.g);
				mColorB.emplace_back(rgb.b);
				mColorA.emplace_back(rgb.a);
			}
		}
	}
}
}