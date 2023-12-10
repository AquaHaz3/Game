#include "Mob.h"
#include "../../core/Scene.h"
#include "../../core/Debug.h"

#include <raymath.h>
#include <rlgl.h>

Mob::Mob(int x, int y, EntityID id)
	: Entity(x, y, 32, 32, id)
{
	isIdle = true;
	target = nullptr;
	texture = Sprite(textures[(int)id]);

	texture.addTile(32, 64, 32, 32); // Go to RIGHT
	texture.addTile(32, 0, 32, 32); // Go to DOWN
	texture.addTile(32, 32, 32, 32); // Go to LEFT
	texture.addTile(32, 96, 32, 32); // Go to UP

	texture.addTile(0, 64, 32, 32); // Go to RIGHT
	texture.addTile(0, 0, 32, 32); // Go to DOWN
	texture.addTile(0, 32, 32, 32); // Go to LEFT
	texture.addTile(0, 96, 32, 32); // Go to UP

	texture.addTile(64, 64, 32, 32); // Go to RIGHT
	texture.addTile(64, 0, 32, 32); // Go to DOWN
	texture.addTile(64, 32, 32, 32); // Go to LEFT
	texture.addTile(64, 96, 32, 32); // Go to UP

	direction = (int) Direction::DOWN;

	detectRadius = 96;
	max_hp = models[(int)id].health;
	health = max_hp;

	idlePos.x = x;
	idlePos.y = y;
	_idle_MoveState = 0;
}

void Mob::Draw()
{

	//if (type == EntityID::Ghost) BeginBlendMode(BLEND_ADD_COLORS);
	texture.DrawTile(aabb.min.x, aabb.min.y, direction + idleTick * 4);
	//if (type == EntityID::Ghost) EndBlendMode();

	if (debug_util::isDebugBoxes()) {
		if (isIdle) {
			DrawCircleLines(aabb.min.x + 16, aabb.min.y + 16, detectRadius, PURPLE);
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

void Mob::Update(__int64 tick)
{
	if (isIdle) {
		if (tick % 144 == 0) idleTick = (idleTick + 1) % 3; // Номер кадра анимации простоя
	};

	if (tick % 3 == 2) {
		chooseTarget(tick);
		toogleState();
	}

	if (target != nullptr) {
		if (tick % 5 == 1) {
			Vector2 z = Vector2MoveTowards(aabb.min, target->aabb.min, 1);
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
			if (d > 64) setPos(z.x, z.y);
		}
	}
	else {
		if (tick % 10 == 1 && (!SceneManager::isSceneStatic())) {
			if (_idle_MoveState == 0) {
				_idle_dx = (1 - (rand() % 3));
				_idle_dy = (1 - (rand() % 3));
				_idle_MoveState = 10;
			}
			else {
				_idle_MoveState--;
			}
			moveBy(_idle_dx, _idle_dy);
			float d = Vector2Distance(aabb.min, idlePos);
			if (d > 64) {
				_idle_dx = -_idle_dx;
				_idle_dy = -_idle_dy;
				moveBy(_idle_dx, _idle_dy);
			}
		}
	}

	if (health <= 0) {
		SceneManager::removeObject(this);
	}
}

#include "../../events/ArrowHitEvent.hpp"
#include "../misc/TextParticle.h"

void Mob::OnEvent(Event* event)
{
	if (event->uuid == ArrowHitEvent::getClassUUID()) {
		ArrowHitEvent* hitEvent = (ArrowHitEvent*)event;
		health -= hitEvent->damage;
		target = hitEvent->arrowOwner;
		hitEvent->destroyArrowAfterHit = true;
		std::string about = "-" + std::to_string((int)hitEvent->damage);
		SceneManager::addParticle(new TextParticle(aabb.max, about, 80, RED));
	}
}

void Mob::chooseTarget(__int64 tick)
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
