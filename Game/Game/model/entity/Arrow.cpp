#include "Arrow.h"

#include "../../GameLauncher.h"
#include "../../core/Debug.h"
#include "../../events/ArrowHitEvent.hpp"

Arrow::Arrow(int x, int y, float speed, float angle, float damage, int id)
	: Entity(x, y, 4, 4)
{
	this->angle = (angle / PI) * 180;
	this->speed = speed;
	this->health = damage; // Здоровье стрелы 🤨 <=> Сколько урона он принесёт

	this->xSpeed = cos(angle) * speed;
	this->ySpeed = sin(angle) * speed;
	lifeTime = 0;
	isMoving = true;
	isAlive = true;
	flags = flags & (~SOLID_OBJECT); // Убираем 'твердость' (чтобы игрок мог проходить сквозь)
}

void Arrow::Draw()
{
	arrow_types[0].DrawPro(aabb.min.x , aabb.min.y, 32, 32, 16, 16, angle);
	if (debug_util::isDebugBoxes()) {
		DrawRectangleLines(aabb.min.x, aabb.min.y, 3, 3, RED);
	}
}

void Arrow::Update(__int64 tick)
{
	if ((lifeTime > 500 || aabb.min.x < 0 || aabb.min.y < 0) && isAlive) {
		GameLauncher::current_scene->removeObject(this);
		isAlive = false;
		return;
	}
	if (tick % 3 == 0) lifeTime++;
	if (!isMoving) return;
	moveBy(xSpeed, ySpeed);
	if (tick % 3 == 2) {
		for (auto solid : GameLauncher::current_scene->boxes) { // Проверяем не столкнулась ли стрела с чем-то?
			if (solid == this) continue;
			if (solid->flags & ENTITY_OBJECT) {
				if (lifeTime < 458) {
					auto e = new ArrowHitEvent(this);
					solid->OnEvent(e); // Отправляем событие 'ArrowHitEvent' объекту, принявшему стрелу
					delete e;
				}
				continue;
			}
			if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
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
}
