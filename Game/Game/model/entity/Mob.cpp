#include "Mob.h"
#include "../../core/Scene.h"
#include "../../core/Debug.h"
#include "Bullet.h"

#include <raymath.h>
#include <rlgl.h>

Mob::Mob(int x, int y, EntityID id)
	: Entity(x, y, 32, 32, id)
{
	isIdle = true;
	target = nullptr;
	texture = textures[(int)id];

	if (!texture->isTiled) {

		texture->addTile(32, 64, 32, 32); // Go to RIGHT
		texture->addTile(32, 0, 32, 32); // Go to DOWN
		texture->addTile(32, 32, 32, 32); // Go to LEFT
		texture->addTile(32, 96, 32, 32); // Go to UP

		texture->addTile(0, 64, 32, 32); // Go to RIGHT
		texture->addTile(0, 0, 32, 32); // Go to DOWN
		texture->addTile(0, 32, 32, 32); // Go to LEFT
		texture->addTile(0, 96, 32, 32); // Go to UP

		texture->addTile(64, 64, 32, 32); // Go to RIGHT
		texture->addTile(64, 0, 32, 32); // Go to DOWN
		texture->addTile(64, 32, 32, 32); // Go to LEFT
		texture->addTile(64, 96, 32, 32); // Go to UP
	}

	direction = (int) Direction::DOWN;

	detectRadius = models[(int)id].detectRadius;
	idleRadius = models[(int)id].idleRadius;
	max_hp = models[(int)id].health;
	isDistanceBattle = models[(int)id].isDistanceBattle;
	isSolid = models[(int)id].solid;
	health = max_hp;

	idlePos.x = x+16;
	idlePos.y = y+16;
	_idle_MoveState = 0;
}

void Mob::Draw()
{

	//if (type == EntityID::Ghost) BeginBlendMode(BLEND_ADD_COLORS);
	texture->DrawTile(aabb.min.x, aabb.min.y, direction + idleTick * 4);
	//if (type == EntityID::Ghost) EndBlendMode();

	if (debug_util::isDebugBoxes()) {
		if (isIdle) {
			DrawCircleLines(aabb.min.x + 16, aabb.min.y + 16, detectRadius, PURPLE);
			DrawCircle(idlePos.x, idlePos.y, 3, YELLOW);
		}
		else {
			DrawCircleLines(aabb.min.x + 16, aabb.min.y + 16, detectRadius, YELLOW);
			if (target != nullptr) {
				DrawLine(aabb.min.x, aabb.min.y, target->aabb.min.x, target->aabb.min.y, GREEN);
			}
		}
		DrawRectangleLines(aabb.min.x, aabb.min.y, w, h, { 250, 20, 20, 200 });
	}

	if (health < max_hp) {
		
	}
}

inline void Mob::updateDirection(float& angle)
{
	if (angle <= 0) angle += 360.0;
	if (angle >= 305.0 || angle <= 45.0) direction = (char)Direction::RIGHT;
	if (angle >= 135.0 && angle <= 235.0) direction = (char)Direction::LEFT;
	if (angle > 45.0 && angle < 135.0) direction = (char)Direction::DOWN;
	if (angle > 235.0 && angle < 305.0) direction = (char)Direction::UP;
}

void Mob::Update(__int64 tick)
{
	//SceneManager::current->notifications++;

	if (tick % 144 == 0 && (_idle_MoveState > 0 || isIdle == false)) idleTick = (idleTick + 1) % 3; // Номер кадра анимации простоя

	if (tick % 3 == 2) {
		chooseTarget(tick);
		toogleState();
	}

	Vector2 prevPos = aabb.min;

	if (target != nullptr) { // Режим преследования цели
		if (tick % 5 == 1) {
			if (tick % 250 == 1 && rand() % 3 == 0) {
				Vector2 center = { aabb.min.x + w / 2,aabb.min.y + h / 2 };
				Vector2 tCenter = { target->aabb.min.x + target->w / 2,target->aabb.min.y + target->h / 2 };
				Vector2 z = Vector2MoveTowards(center, tCenter, 64);
				float _angle = MyVector2Angle({ center.x + 1, center.y }, z);
				Bullet::SpawnMobBullet(type, center.x, center.y, _angle, this);
			}
			else {
				Vector2 z = Vector2MoveTowards(aabb.min, target->aabb.min, 1);
				Vector2 dz = Vector2Subtract(aabb.min, z);
				float d = Vector2Distance(aabb.min, target->aabb.min);
				for (auto solid : SceneManager::current->boxes) {
					if (solid->flags & ENTITY_OBJECT) {
						if (solid == this) continue;
						float d = Vector2Distance(aabb.min, solid->aabb.min);
						if (d < 20) {
							Vector2 z2 = Vector2MoveTowards(aabb.min, solid->aabb.min, 1);
							Vector2 z3 = Vector2Subtract(aabb.min, z2);
							moveBy(z3.x, -z3.y);
							return;
						}
					}
				}
				float angle = (MyVector2Angle({ aabb.min.x - 1, aabb.min.y }, z) / PI) * 360.0;
				updateDirection(angle);
				if (isDistanceBattle) {
					if (d < detectRadius * 2) {
						if (dz.x > 0) moveBy(dz.x, -dz.y);
						if (dz.x < 0) moveBy(-dz.x, dz.y);
					}
				}
				else {
					if (d > 64) setPos(z.x, z.y);
				}
			}
		}
	}
	else { // Режим простоя
		if (tick % 10 == 1 && (!SceneManager::isSceneStatic())) {
			if (_idle_MoveState == 0) {
				_idle_dx = (1 - (rand() % 3));
				_idle_dy = (1 - (rand() % 3));
				_idle_MoveState = 10;
				if(_idle_dx == _idle_dy && _idle_dx == 0){
					_idle_MoveState = 30;
				}
			}
			else {
				_idle_MoveState--;
			}
			moveBy(_idle_dx, _idle_dy);
			float d = Vector2Distance(aabb.min, idlePos);
			if (d > idleRadius) {
				_idle_dx = -_idle_dx;
				_idle_dy = -_idle_dy;
				moveBy(_idle_dx, _idle_dy);
			}
		}
	}

	if (isSolid) { // Если не может проникать через стены, проверяем
		for (auto solid : SceneManager::current->boxes) {
			if (solid->flags & SOLID_OBJECT) {
				if (solid == this) continue;
				if (UtilAABB::isOverlap(&aabb, &solid->aabb)) {
					setPos(prevPos.x, prevPos.y);
					break;
				};
			}
		}
	}

	if (health <= 0) {
		SceneManager::removeObject(this);
	}
}

#include "../../events/ProjectileHitEvent.hpp"
#include "../misc/TextParticle.h"
#include "../misc/AnimatedParticle.h"
#include "../ui/SoundUI.h"

void Mob::OnEvent(Event* event)
{
	if (event->uuid == ProjectileHitEvent::getClassUUID()) {
		ProjectileHitEvent* hitEvent = (ProjectileHitEvent*)event;
		health -= hitEvent->damage; // урон
		target = hitEvent->arrowOwner;
		hitEvent->destroyArrowAfterHit = true;
		std::string about = "-" + std::to_string((int)hitEvent->damage);
		SceneManager::addParticle(new TextParticle(aabb.max, about, 80, RED));
		if (hitEvent->isExplosiveArrow) {
			Vector2 pos = { (float)hitEvent->x, (float)hitEvent->y };
			SceneManager::addParticle(new AnimatedParticle(pos, AnimationID::SharpExplosion, RED));
			SoundUI::PlayOnce("explosion_small");
		}
	}
}

void Mob::chooseTarget(__int64 tick) // Выбор цели (для атаки)
{
	auto players = SceneManager::GetPlayers();

	bool newTarget = false;

	if (target == nullptr) {
		for (auto player : players) {
			Vector2 pos = player->getPos();
			float d = Vector2Distance(aabb.min, pos);
			if (d < detectRadius) {
				target = player;
				newTarget = true;
				break;
			}
		}
	}

	if (target != nullptr && (tick % 18 == 2 || newTarget)) {

		for (auto solid : SceneManager::current->boxes) { // Проходим по всем твёрдым предметам
			if (!(solid->flags & ENTITY_OBJECT)) {
				if (UtilAABB::isIntersectsBox(&solid->aabb, aabb.min, target->aabb.min)) {
					target = nullptr;
					return;
				};
			}
		}

	}
}

void Mob::toogleState()
{
	isIdle = (target == nullptr);
	if (isIdle == false) {
		idlePos.x = aabb.min.x;
		idlePos.y = aabb.min.y;
	}
}
