#include "AnimatedParticle.h"

AnimatedParticle::AnimatedParticle() : Particle() 
{
	this->anim = 0;
	this->x = 0;
	this->y = 0;
	this->color = {0,0,0,0};
	this->id = (uint16_t)0;
}

AnimatedParticle::AnimatedParticle(Vector2 pos, AnimationID id, Color color)
	: Particle()
{
	this->anim = &animations[id];
	this->x = pos.x - anim->sprite->tile[0]->width / 2;
	this->y = pos.y - anim->sprite->tile[0]->height / 2;
	this->color = color;
	this->id = (uint16_t) id;
}

void AnimatedParticle::Draw()
{
	if (life_tick < anim->frameCount) {
		anim->sprite->DrawTile(x, y, anim->startFrame + life_tick);
	}
}

void AnimatedParticle::Update(__int64 tick)
{
	if (tick % 20 != 1) return;
	life_tick++;
	if (life_tick > anim->frameCount) {
		isAlive = false;
	}
}

std::map<AnimationID, ParticleAnimation> AnimatedParticle::animations = std::map<AnimationID, ParticleAnimation>();

void AnimatedParticle::addAnimatedEffect(
	AnimationID id, std::string sprite_name, 
	int tW, int tH, int rows, int cols, int startPosition,
	bool isVertical
)
{
	auto sprite = SpriteLoader::GetSprite(sprite_name);

	int animVector = isVertical ? cols : rows; // разные анимации
	int framesVector = isVertical ? rows : cols; // кадры

	int step = isVertical ? tH : tW; // кадры

	if (!sprite->isTiled) {
//		sprite->tile = std::vector<Rectangle>(rows * cols + 1);
		for (int i = 0; i < animVector; i++) {
			for (int j = 0; j < framesVector; j++) {
				sprite->addTile(j * tW, i * tH, tW, tH);
			}
		}
	}

	auto anim = ParticleAnimation(sprite.get(), startPosition * framesVector, framesVector, isVertical);

	animations.emplace(id, anim);
}

void AnimatedParticle::InitAnimatedEffects()
{
	addAnimatedEffect(AnimationID::SharpExplosion, "effect/Pack1Yellow.png", 64, 64, 24, 8, 11, false);
}

ParticleAnimation::ParticleAnimation()
{
	this->sprite = 0;
	this->startFrame = 0;
	this->frameCount = 0;
	this->isVertical = false;
}
