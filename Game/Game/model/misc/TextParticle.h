#pragma once

#include "../../core/Particle.hpp"
#include <string>
#include <raylib.h>

class TextParticle : public Particle
{

public:

	TextParticle();
	~TextParticle();

	/* ������� ������ (���������, � ����� ��� ������, ���������� �������)
	@param pos - ������� ������; 
	@param text - �����;
	@param maxLifeTime - ����� ����� �������;
	@param color - ����;
	*/
	TextParticle(Vector2 pos, std::string text, int maxLifetime, Color color);

	virtual void Draw();
	virtual void Update(__int64 tick);
	virtual void OnEvent(Event* e) {};

private:

	uint16_t x;
	uint16_t y;
	std::string text;
	Color color;
	int maxLifetime;
	int stepWeight;

};

