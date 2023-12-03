#include "TextParticle.h"

TextParticle::TextParticle() : Particle()
{
	this->x = 0;
	this->y = 0;
	this->text = "";
	this->color = { 0,0,0,0 };
	this->maxLifetime = 0;
}

TextParticle::~TextParticle()
{
	text.~basic_string();
}

TextParticle::TextParticle(Vector2 pos, std::string text, int maxLifetime, Color color)
	: Particle()
{
	this->x = pos.x;
	this->y = pos.y;
	this->text = std::move(text);
	this->color = std::move(color);
	this->maxLifetime = maxLifetime;
	stepWeight = 255 / maxLifetime;
}

void TextParticle::Draw()
{
	DrawText(text.c_str(), x+1, y - life_tick + 1, 11, { 30, 30, 30, (uint8_t)(255 - life_tick * stepWeight) });
	DrawText(text.c_str(), x, y - life_tick, 11, { color.r, color.g, color.b, (uint8_t)(255 - life_tick*stepWeight) });
}

void TextParticle::Update(__int64 tick)
{
	if (tick % 9 != 1) return;
	life_tick++;
	if (life_tick > maxLifetime) {
		isAlive = false;
	}
}
