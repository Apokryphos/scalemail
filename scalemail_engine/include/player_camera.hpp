#pragma once

namespace ScaleMail
{
class Game;
class World;

void activatePlayerCamera(Game& game);
void createPlayerCamera(World& world);
void setPlayerCameraFollowEntity(Entity entity, Game& game,
								 bool activateCamera = false);
}