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

	/* ������� ������ (���������, � ����� ��� ������, ���������� �������)
	@param pos - ������� ������;
	@param id - id;
	@param maxLifeTime - ����� ����� �������;
	@param color - ����;
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
	@param sprite_name - ��� �������
	@param tW - ������ ������ �����; @param tH - ������ ������ �����
	@param rows - ���-�� ����� ������; @param cols - ���-�� ��������
	@param startFrame - ��������� ������� �� (�������� ��� �������)
	@param isVertial - ����������� ������ (������� ��� �������)
	*/
	static void addAnimatedEffect(AnimationID id, std::string sprite_name,
		int tW, int tH, int rows, int cols, int startPosition,
		bool isVertical);

};