#include "EnemyController.h"

void EnemyController::Update(Entity& entity, Tmpl8::vec2 moveBy/*, Tmpl8::vec2 TileMapOffset*/)
{
	entity.SetPosition(entity.GetPosition() + moveBy);
}
