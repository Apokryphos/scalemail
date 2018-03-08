#pragma once

namespace ScaleMail
{
struct Entity;
class World;

//	Actor utility functions
//	These are game rules logic functions that check multiple systems to see if
//	actions are legal.

bool actorCanFire(const Entity& entity, World& world);
bool actorCanMove(const Entity& entity, World& world);
bool actorIsAlive(const Entity& entity, World& world);
}