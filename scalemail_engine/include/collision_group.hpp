#pragma once

namespace ScaleMail
{
enum class CollisionGroup : uint8_t
{
	NONE = 			0,
	STATIC = 		(1 << 1),
	ACTOR = 		(1 << 2),
	BULLET = 		(1 << 3),
	ITEM = 			(1 << 4),
	PLAYER_ACTOR =  (1 << 5),
	PLAYER_BULLET = (1 << 6),
	TRIGGER = 		(1 << 7)
};

inline CollisionGroup operator |(const CollisionGroup selfValue, const CollisionGroup inValue)
{
	return static_cast<CollisionGroup>(static_cast<uint8_t>(selfValue) | static_cast<uint8_t>(inValue));
}

inline CollisionGroup operator &(const CollisionGroup selfValue, const CollisionGroup inValue)
{
	return static_cast<CollisionGroup>(static_cast<uint8_t>(selfValue) & static_cast<uint8_t>(inValue));
}

inline CollisionGroup getCollisionFilter(CollisionGroup group)
{
	switch (group)
	{
	case CollisionGroup::NONE:
		return CollisionGroup::NONE;

	case CollisionGroup::STATIC:
		return CollisionGroup::ACTOR | CollisionGroup::PLAYER_BULLET;

	case CollisionGroup::ACTOR:
		return
			CollisionGroup::STATIC |
			CollisionGroup::ACTOR |
			CollisionGroup::PLAYER_ACTOR |
			CollisionGroup::PLAYER_BULLET;

	case CollisionGroup::ITEM:
		return CollisionGroup::PLAYER_ACTOR;

	case CollisionGroup::PLAYER_ACTOR:
		return CollisionGroup::STATIC | CollisionGroup::ACTOR | CollisionGroup::BULLET;

	case CollisionGroup::BULLET:
		return CollisionGroup::STATIC | CollisionGroup::PLAYER_ACTOR;

	case CollisionGroup::PLAYER_BULLET:
		return CollisionGroup::STATIC | CollisionGroup::ACTOR;

	case CollisionGroup::TRIGGER:
		return
			CollisionGroup::ACTOR |
			CollisionGroup::PLAYER_ACTOR |
			CollisionGroup::BULLET |
			CollisionGroup::PLAYER_BULLET;

	default:
		throw new std::runtime_error("Filter not implemented.");
	}
}
}