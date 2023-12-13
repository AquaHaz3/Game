#include "Arrow.h"

#include "../../core/Debug.h"
#include "../../events/ArrowHitEvent.hpp"
#include "../../core/Scene.h"

Arrow::Arrow() : Entity(0,0,0,0)
{
	this->angle = 0;
	this->speed = 0;
	this->health = 0;

	this->xSpeed = 0;
	this->ySpeed = 0;
	lifeTime = 0;
	isMoving = false;
	isAlive = false;
	flags = flags & (~SOLID_OBJECT); // Убираем 'твердость' (чтобы игрок мог проходить сквозь)
	this->owner = 0;
	id = 0;
}

Arrow::Arrow(int x, int y, float speed, float angle, Weapon* weapon, Entity* owner)
	: Entity(x, y, 4, 4)
{
	this->angle = (angle / PI) * 180;
	this->speed = speed;
	this->health = (int)(weapon->damage * (speed - 0.5f)); // Здоровье стрелы 🤨 <=> Сколько урона он принесёт

	this->xSpeed = (float)cos(angle) * speed;
	this->ySpeed = (float)sin(angle) * speed;
	lifeTime = 0;
	isMoving = true;
	isAlive = true;
	flags = flags & (~SOLID_OBJECT); // Убираем 'твердость' (чтобы игрок мог проходить сквозь)
	this->owner = owner;

	id = 0;
	if (weapon->level == (int)WLevel::LEGENDARY) {
		id = 1;
	}
}

void Arrow::Draw()
{
	arrow_types[id].DrawPro(aabb.min.x , aabb.min.y, 32, 32, 16, 16, angle);
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, 3, 3, RED);
	}
}

void Arrow::Update(__int64 tick)
{
	if ((lifeTime > 500 || aabb.min.x < 0 || aabb.min.y < 0) && isAlive) {
		SceneManager::removeObject(this);
		isAlive = false;
		return;
	}
	if (tick % 3 == 0) lifeTime++;
	if (!isMoving) return;
	moveBy(xSpeed, ySpeed);
	if (tick % 3 == 2) {
		for (auto solid : SceneManager::current->boxes) { // Проверяем не столкнулась ли стрела с чем-то?
			if (solid == this) continue;
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
				if (solid->flags & ENTITY_OBJECT) {
					bool phaseThrow = false;
					if (lifeTime < 458) {
						auto e = new ArrowHitEvent(this);
						solid->OnEvent(e); // Отправляем событие 'ArrowHitEvent' объекту, принявшему стрелу
						if (e->destroyArrowAfterHit) lifeTime = 501;
						phaseThrow = e->phase;
						delete e;
					}
					if (phaseThrow) continue;
				}
				isMoving = false;
				return;
			}
		}
	}
}

void Arrow::OnEvent(Event* event)
{
}

std::vector<Sprite> Arrow::arrow_types = std::vector<Sprite>();

void Arrow::InitArrows()
{
	arrow_types.push_back(Sprite("arrow.png"));
	arrow_types.push_back(Sprite("arrow2.png"));
}
