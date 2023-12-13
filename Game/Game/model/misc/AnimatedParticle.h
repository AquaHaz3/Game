#pragma once

#include "../../core/Particle.hpp"
#include <string>
#include <raylib.h>
#include <map>
#include "../../core/Sprite.h"

enum class AnimationID {

	SharpExplosion,

};

struct ParticleAnimation {

	Sprite* sprite;
	bool isVertical;
	uint8_t frameCount;
	uint16_t startFrame;

	ParticleAnimation(Sprite* sprite, int startFrame, int frameCount ,bool isVertical) 
	{
		this->sprite = sprite;
		this->startFrame = startFrame;
		this->frameCount = frameCount;
		this->isVertical = isVertical;
	}

	ParticleAnimation();;

};

class AnimatedParticle : public Particle
{

public:

	AnimatedParticle();

	/* Частица текста (Спавнится, а потом идёт наверх, постепенно исчезая)
	@param pos - позиция спавна;
	@param id - id;
	@param maxLifeTime - время жизни частицы;
	@param color - цвет;
	*/
	AnimatedParticle(Vector2 pos, AnimationID id, Color color);

	virtual void Draw();
	virtual void Update(__int64 tick);
	virtual void OnEvent(Event* e) {};

	static void InitAnimatedEffects();

private:

	uint16_t x;
	uint16_t y;
	uint16_t id;
	Color color;
	ParticleAnimation* anim;

protected:

	static std::map<AnimationID, ParticleAnimation> animations;

	/* 
	@param id - id
	@param sprite_name - имя спрайта
	@param tW - ширина одного кадра; @param tH - высота одного кадра
	@param rows - кол-во строк кадров; @param cols - кол-во столбцов
	@param startFrame - стартовая позиция из (столбцов или строчек)
	@param isVertial - нарпавление кадров (столбцы или строчки)
	*/
	static void addAnimatedEffect(AnimationID id, std::string sprite_name,
		int tW, int tH, int rows, int cols, int startPosition,
		bool isVertical);

};