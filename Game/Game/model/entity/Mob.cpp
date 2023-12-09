#include "Mob.h"
#include "../../core/Scene.h"

Mob::Mob(int x, int y, EntityID id)
	: Entity(x, y, 32, 32)
{

}

void Mob::Draw()
{
	
}

void Mob::Update(__int64 tick)
{
}

Entity* Mob::chooseTarget()
{
	return SceneManager::current->player.get();
}
