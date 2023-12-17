#pragma once

#include "Block.h"

#include "../events/ProjectileHitEvent.hpp"
#include "misc/AnimatedParticle.h"
#include "../core/Scene.h"
#include "ui/SoundUI.h"

class Wall : public Box2D
{

public:

	short blocksW;
	short blocksH;
	char id;

public:

	Wall() { blocksW = 0; blocksH = 0; id = 0; };
	Wall(int x, int y, int x2, int y2, BlockID id);

	virtual void Draw();

	virtual void Update(__int64 tick) { }

	virtual void OnEvent(Event* event) {
		if (event->uuid == ProjectileHitEvent::getClassUUID()) {
			ProjectileHitEvent* hitEvent = (ProjectileHitEvent*)event;
			if (hitEvent->isExplosiveArrow) {
				Vector2 pos = { (float)hitEvent->x, (float)hitEvent->y };
				SceneManager::addParticle(new AnimatedParticle(pos, AnimationID::SharpExplosion, RED));
				SoundUI::PlayOnce("explosion_small");
			}
		}
	};

};

